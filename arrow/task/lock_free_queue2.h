#pragma once
#include <list>
#include <functional>
#include <tuple>
#include <thread>
#include <iostream>
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <string>
#include <pthread.h>
#include <future>
#include <vector>
#include <atomic>
#include "../other/std_assist.h"

namespace Arrow
{

namespace Pattern
{

/*
1.代码实现 完成
2.SPSC模式 x86_64:测试通过 ARM64:未测试 ARM32:未测试
3.MPSC模式 x86_64:测试通过 ARM64:未测试 ARM32:未测试
4.SPMC模式 x86_64:未测试 ARM64:未测试 ARM32:未测试
5.MPMC模式 x86_64:未测试 ARM64:未测试 ARM32:未测试
6.性能测试 x86_64:未测试 ARM64:未测试 ARM32:未测试
*/
template <typename T>
struct SelectTaggedPtr
{
    uint32_t u32Tag = 0;
    T* pNode = nullptr;

    SelectTaggedPtr() noexcept : u32Tag(0), pNode(nullptr) {}

    SelectTaggedPtr(const SelectTaggedPtr& val) = default;

    explicit SelectTaggedPtr(T* p, uint32_t tag = 0) : pNode(p), u32Tag(tag) {}

    SelectTaggedPtr& operator=(const SelectTaggedPtr& val) = default;

    bool operator==(volatile const SelectTaggedPtr& val) const
    {
        return pNode == val.pNode && u32Tag == val.u32Tag;
    }

    bool operator!=(volatile const SelectTaggedPtr& val) const
    {
        return !operator==(val);
    }
};

// 无锁队列，无锁队列必须有一个哑结点。否则在处理插入第一个节点和最后一个节点的时候会出现重入问题 [zhuyb 2023-10-15 17:24:01]
template<typename ...Args>
class LockFreeQueue2
{
    constexpr static const int32_t MAX_THREAD_COUNT = 32;
    constexpr static const int32_t MAX_DELETE_NODE_CACHE_COUNT = 2;
    using Local = LockFreeQueue2<Args...>;
    using Data = std::tuple<Args...>;

    struct Node
    {
        using TaggedPtr = SelectTaggedPtr<Node>;
        Data* pValue = nullptr;
        std::atomic<TaggedPtr> next;

        Node() = default;

        Node(const Node&) = default;

        Node(Data* pVal) : pValue(pVal)
        {
        }

        ~Node()
        {
            if (pValue != nullptr)
                delete pValue;
        }
    };

    using TaggedPtr = typename Node::TaggedPtr;

public:
    LockFreeQueue2()
    {
        m_u64CreateCount++;
        Node* pDummyNode = new Node(nullptr);
        TaggedPtr taggedDummyNode(pDummyNode, 0);
        m_Head.store(taggedDummyNode, std::memory_order_relaxed);
        m_Tail.store(taggedDummyNode, std::memory_order_relaxed);
        m_strQueueName = "LockFreeQueue2";
    }
    LockFreeQueue2(const Local&) = delete;

    virtual ~LockFreeQueue2()
    {
        // for(;;)
        // {
        //     TaggedPtr head = m_Head.load(std::memory_order_acquire);
        //     Node* pHeadNode = head.pNode;

        //     TaggedPtr tail = m_Tail.load(std::memory_order_acquire);
        //     Node* pTailNode = tail.pNode;

        //     TaggedPtr next = pHeadNode->next.load(std::memory_order_acquire);
        //     Node* pNextNode = next.pNode;

        //     if (head == tail)
        //     {
        //         if (pNextNode == nullptr)
        //         {
        //             std::cout << __LINE__ << " " << "End of queue " << std::endl;
        //             delete pHeadNode;
        //             m_u64DeleteCount++;
        //             break;
        //         }

        //         TaggedPtr newTail(pNextNode, head.u32Tag + 1);
        //         m_Tail.compare_exchange_strong(tail, newTail);
        //     }
        //     else
        //     {
        //         TaggedPtr newHead(pNextNode, head.u32Tag + 1);
        //         if (m_Head.compare_exchange_strong(head, newHead) == true)
        //         {
        //             delete pHeadNode;
        //             m_u64DeleteCount++;
        //         }
        //     }
        // }

        while (CallPopHelper<Args...>())
        {
            
        }

        delete m_Tail.load().pNode;
        m_u64DeleteCount++;

        for (auto& it : m_ListDeletedNodes)
        {
            delete it.pNode;
            m_u64DeleteCount++;
        }
        m_ListDeletedNodes.clear();

        m_mapHazardPointers.clear();

        std::cout << __LINE__ << " " << "LockFreeQueue2 Create:Delete:Diff "
                  << m_u64CreateCount.load() << ":"
                  << m_u64DeleteCount.load() << ":"
                  << m_u64CreateCount.load() - m_u64DeleteCount.load() << std::endl;
    }

    bool Push(Args... args)
    {
        std::atomic<TaggedPtr>* pHazardPointers = GetHazardPointers();

        // 创建一个新节点 [zhuyb 2024-08-21 11:09:44]
        m_u64CreateCount++;
        Data* pNewValue = new Data(std::forward<Args>(args)...);
        Node* pNewNode = new Node(pNewValue);
        
        for(;;)
        {
            pHazardPointers->store(m_Tail, std::memory_order_release);
            TaggedPtr tail = m_Tail.load(std::memory_order_acquire);
            Node* pTailNode = tail.pNode;         

            TaggedPtr next = pTailNode->next.load(std::memory_order_acquire);
            Node* pNextNode = next.pNode;

            TaggedPtr tail2 = m_Tail.load(std::memory_order_acquire);
            
            if(tail != tail2)
            {
                continue;
            }

            if (pNextNode == nullptr)
            {
                TaggedPtr newNext(pNewNode, tail.u32Tag + 1);
                if (pTailNode->next.compare_exchange_weak(next, newNext) == true)
                {
                    TaggedPtr newTail(pNewNode, tail.u32Tag + 1);
                    m_Tail.compare_exchange_strong(tail, newTail);
                    m_u32Count++;
                    pHazardPointers->store(TaggedPtr(nullptr, 0), std::memory_order_release);
                    return true;
                }
            }
            else
            {
                TaggedPtr newTail(pNextNode, tail.u32Tag + 1);
                m_Tail.compare_exchange_strong(tail, newTail);
            }
        }

        return false;
    }

    bool Pop(Args&... args)
    {
        std::atomic<TaggedPtr>* pHazardPointers = GetHazardPointers();
        Data* pRetValue = nullptr;
        for (;;)
        {
            pHazardPointers->store(m_Head, std::memory_order_release);
            TaggedPtr head = m_Head.load(std::memory_order_acquire);
            Node* pHeadNode = head.pNode;

            TaggedPtr tail = m_Tail.load(std::memory_order_acquire);
            Node* pTailNode = tail.pNode;

            TaggedPtr next = pHeadNode->next.load(std::memory_order_acquire);
            Node* pNextNode = next.pNode;

            TaggedPtr head2 = m_Head.load(std::memory_order_acquire);

            if(head != head2)
            {
                continue;
            }

            // if(pHeadNode == pTailNode)
            if(head == tail)
            {
                if(pNextNode == nullptr)
                {
                    pHazardPointers->store(TaggedPtr(nullptr, 0), std::memory_order_release);
                    return false;
                }

                TaggedPtr newTail(pNextNode, head.u32Tag + 1);
                m_Tail.compare_exchange_strong(tail, newTail);
            }
            else
            {
                if(pNextNode == nullptr)
                {
                    continue;
                }

                pRetValue = pNextNode->pValue;

                TaggedPtr newHead(pNextNode, head.u32Tag + 1);
                if(m_Head.compare_exchange_strong(head, newHead) == true)
                {
                    std::tie(args...) = *(pRetValue);
                    // delete head.pNode;
                    pHazardPointers->store(TaggedPtr(nullptr, 0), std::memory_order_release);
                    SafeDeleteHazardPointers(head);
                    m_u32Count--;
                    return true;
                }
            }
        }
    }

    uint32_t Size()
    {
        return m_u32Count.load();
    }

    bool Empty()
    {
        return m_Head.load().pNode == m_Tail.load().pNode;
    }

    void SetName(const char* szName)
    {
        m_strQueueName = szName;
    }

    void TestPrintQueue()
    {
        int32_t n32ReadCount = 0;
        TaggedPtr head = m_Head.load(std::memory_order_acquire);
        Node* pHeadNode = head.pNode;

        TaggedPtr tail = m_Tail.load(std::memory_order_acquire);
        Node* pTailNode = tail.pNode;
        if (head == tail)
        {
            std::cout << __LINE__ << " " << "tag:" << tail.u32Tag << " Node: " << tail.pNode->next.load().pNode << std::endl;
            std::cout << __LINE__ << " " << "catch:read " << m_u32Count.load() << ":" << n32ReadCount << std::endl;
            return;
        }

        TaggedPtr curl = head;

        for (;;)
        {
            if (curl == tail)
            {
                std::cout << __LINE__ << " " << "tag:" << curl.u32Tag << " Node: " << tail.pNode->next.load().pNode << std::endl;
                std::cout << __LINE__ << " " << "catch:read " << m_u32Count.load() << ":" << n32ReadCount << std::endl;
                return;
            }

            std::cout << __LINE__ << " " << "tag:" << curl.u32Tag << std::endl;
            if(curl.pNode == nullptr) 
            {
                std::cout << __LINE__ << " " << "Error catch:read " << m_u32Count.load() << ":" << n32ReadCount << std::endl;
                break;
            }
            curl = curl.pNode->next.load(std::memory_order_acquire);
            n32ReadCount++;
        }
        
    }

private:
    std::atomic<TaggedPtr>* GetHazardPointers()
    {
        uint64_t u64ThreadID = Arrow::Other::GetThreadID();

        if(m_u32HazardPointerIndex.load() >= (int32_t)m_arrHazardPointers.size())
        {
            std::cout << __LINE__ << " " << "There are too many threads, exceeding the upper limit of " << m_arrHazardPointers.size() << std::endl;
            return nullptr;
        }
        std::lock_guard<std::mutex> lock(m_mutexMapHazardPointers);
        auto it = m_mapHazardPointers.find(u64ThreadID);
        if(it == m_mapHazardPointers.end())
        {
            m_mapHazardPointers[u64ThreadID] = m_u32HazardPointerIndex.load();
            m_u32HazardPointerIndex++;
        }
        return &m_arrHazardPointers[m_mapHazardPointers[u64ThreadID]];
    }

    void SafeDeleteHazardPointers(TaggedPtr& taggedPtr)
    {
        m_mutexListDeletedNodes.lock();
        m_ListDeletedNodes.push_back(taggedPtr);
        if(m_ListDeletedNodes.size() < MAX_DELETE_NODE_CACHE_COUNT)
        {
            m_mutexListDeletedNodes.unlock();
            return;
        }
        TaggedPtr delTaggedPtr = m_ListDeletedNodes.front();
        m_ListDeletedNodes.pop_front();
        m_mutexListDeletedNodes.unlock();
    
        bool bDelete = false;
        m_u64Yield1++;
        for(;;)
        {
            bDelete = true;
            for (auto it = m_arrHazardPointers.begin(); it != m_arrHazardPointers.end(); it++)
            {
                if (it->load(std::memory_order_acquire) == delTaggedPtr)
                {
                    bDelete = false;
                    break;
                }
            }
            if (bDelete == true)
            {
                delete delTaggedPtr.pNode;
                m_u64DeleteCount++;
                return;
            }
            else
            {
                m_u64Yield2++;
                // std::cout << __LINE__ << " " << "There are still hazard pointers, waiting for them to be released."
                //      << m_u64Yield2.load() << "/" << m_u64Yield1.load() << " = " 
                //      << (100.0 * m_u64Yield2.load() / m_u64Yield1.load()) << "%" << std::endl;
                std::this_thread::sleep_for(std::chrono::microseconds(1));
            }
        }
    }

    // 辅助函数：用于在析构函数中调用 Pop
    template<typename... T>
    bool CallPopHelper()
    {
        // 创建默认构造的参数
        std::tuple<T...> args; // 使用 std::tuple 存储参数
        return CallPopImpl(args, std::index_sequence_for<T...>{}); // 展开参数
    }

    // 实现函数：将参数从 tuple 中解包并传递给 Pop
    template<typename Tuple, std::size_t... I>
    bool CallPopImpl(Tuple& args, std::index_sequence<I...>)
    {
        return Pop(std::get<I>(args)...); // 调用 Pop
    }
private:
    std::atomic<TaggedPtr> m_Head;    // 链表头部 [zhuyb 2024-08-21 11:09:07]
    std::atomic<TaggedPtr> m_Tail;    // 链表尾部 [zhuyb 2024-08-21 11:09:24]
    std::atomic<uint32_t> m_u32Count{0};    // 链表个数 [zhuyb 2024-08-21 11:09:30]
    std::string m_strQueueName;
    
    // Hazard Pointer 用于节点释放 [zhuyb 2025-02-06 16:29:51]
    std::array<std::atomic<TaggedPtr>, MAX_THREAD_COUNT> m_arrHazardPointers;
    std::mutex m_mutexMapHazardPointers;
    std::atomic<int32_t> m_u32HazardPointerIndex{0};
    std::map<uint64_t, int32_t> m_mapHazardPointers;

    std::mutex m_mutexListDeletedNodes;
    std::list<TaggedPtr> m_ListDeletedNodes;    // 缓存指定个数节点，用于释放，减少自旋 [zhuyb 2025-02-06 16:30:29]

    // test [zhuyb 2025-02-06 15:34:42]
    std::atomic<uint64_t> m_u64Yield1{0};
    std::atomic<uint64_t> m_u64Yield2{0};

    std::atomic<uint64_t> m_u64CreateCount{0};
    std::atomic<uint64_t> m_u64DeleteCount{0};
};

}
}
