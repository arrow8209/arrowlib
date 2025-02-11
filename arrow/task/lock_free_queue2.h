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
#include <vector>
#include "tagged_point.h"
#include "../other/std_assist.h"

namespace Arrow
{

namespace Pattern
{


// 无锁队列，无锁队列必须有一个哑结点。否则在处理插入第一个节点和最后一个节点的时候会出现重入问题 [zhuyb 2023-10-15 17:24:01]
// 存在性能问题，需要优化 [zhuyb 2025-02-08 14:14:28]
template<int32_t MaxThreadCount, int32_t MaxDeleteNodeCacheCount, typename ...Args>
class LockFreeQueueV2
{
    constexpr static const int32_t HAZARD_POINTERS_COUNT = 2; // 每个线程函数 hazard pointer 数量
    using Local = LockFreeQueueV2<MaxThreadCount, MaxDeleteNodeCacheCount, Args...>;
    using Data = std::tuple<Args...>;

    struct Node
    {
        using TaggedNodePtr = TaggedPtr<Node>;
        Data* pValue = nullptr;
        std::atomic<TaggedNodePtr> next;

        Node() = default;

        Node(const Node&) = default;

        Node(Data* pVal) : pValue(pVal)
        {
        }

        ~Node()
        {
        }
    };

public:
    using TaggedNodePtr = typename Node::TaggedNodePtr;

public:
    LockFreeQueueV2()
    {
        m_u64CreateCount++;
        Node* pDummyNode = new Node(nullptr);
        TaggedNodePtr taggedDummyNode(pDummyNode, 0);
        m_Head.store(taggedDummyNode, std::memory_order_relaxed);
        m_Tail.store(taggedDummyNode, std::memory_order_relaxed);
        m_strQueueName = "LockFreeQueueV2";
    }
    LockFreeQueueV2(const Local&) = delete;

    virtual ~LockFreeQueueV2()
    {
        // 清空队里所有节点 [zhuyb 2025-02-08 14:14:28]
        while (CallPopHelper<Args...>())
        {
            
        }

        // 删除哑结点 [zhuyb 2025-02-08 14:14:28]
        delete m_Tail.load().pNode;
        m_u64DeleteCount++;

        // 删除缓存删除的节点 [zhuyb 2025-02-08 14:14:54]
        for (auto& it : m_ListDeletedNodes)
        {
            delete it.pNode;
            m_u64DeleteCount++;
        }

        // 打印统计信息 [zhuyb 2025-02-08 14:14:54]
        std::cout << __LINE__ << " " << "LockFreeQueue2 Create:Delete:Diff "
                  << m_u64CreateCount.load() << ":"
                  << m_u64DeleteCount.load() << ":"
                  << m_u64CreateCount.load() - m_u64DeleteCount.load() << std::endl;
    }

    bool Push(Args... args)
    {
        std::atomic<TaggedNodePtr>* pHazardPointers1 = nullptr;
        std::atomic<TaggedNodePtr>* pHazardPointers2 = nullptr;
        if(GetHazardPointers(pHazardPointers1, pHazardPointers2) == false)
        {
            return false;
        }

        // 创建一个新节点 [zhuyb 2024-08-21 11:09:44]
        m_u64CreateCount++;
        Data* pNewValue = new Data(std::forward<Args>(args)...);
        Node* pNewNode = new Node(pNewValue);
        
        for(;;)
        {
            // 获取尾节点 并将尾节点加入到 HazardPointers 中 [zhuyb 2024-08-21 11:09:44]
            TaggedNodePtr tail = m_Tail.load(std::memory_order_acquire);
            pHazardPointers1->store(tail, std::memory_order_release);

            // 判断尾节点是否变更，保证 HazardPointers 中爆出你的尾节点为有效的 [zhuyb 2025-02-08 14:25:11]
            if(m_Tail.load(std::memory_order_acquire) != tail)
            {
                std::this_thread::yield();
                continue;
            }

            TaggedNodePtr next = tail.pNode->next.load(std::memory_order_acquire);
            if (next.pNode == nullptr)
            {
                // 尾尾节点中添加 next 节点 [zhuyb 2025-02-08 14:26:52]
                TaggedNodePtr newNext(pNewNode, tail.u32Tag + 1);
                if (tail.pNode->next.compare_exchange_weak(next, newNext) == true)
                {
                    // 更新尾节点 [zhuyb 2025-02-08 14:26:52]
                    TaggedNodePtr newTail(pNewNode, tail.u32Tag + 1);
                    m_Tail.compare_exchange_strong(tail, newTail);
                    m_u32Count++;
                    // 放弃尾节点 HazardPointers [zhuyb 2025-02-08 14:26:52]
                    pHazardPointers1->store(TaggedNodePtr(), std::memory_order_release);
                    return true;
                }
            }
            else
            {
                // 尾节点中 存在next 推进尾节点 [zhuyb 2025-02-08 14:26:12]
                TaggedNodePtr newTail(next.pNode, tail.u32Tag + 1);
                m_Tail.compare_exchange_strong(tail, newTail);
            }
        }

        return false;
    }

    bool Pop(Args&... args)
    {
        std::atomic<TaggedNodePtr>* pHazardPointers1 = nullptr;
        std::atomic<TaggedNodePtr>* pHazardPointers2 = nullptr;
        if(GetHazardPointers(pHazardPointers1, pHazardPointers2) == false)
        {
            return false;
        }

        Data* pRetValue = nullptr;
        TaggedNodePtr nodeToDelete;
        for (;;)
        {
            //初始化数据
            pRetValue = nullptr;

            // 获取头节点 并将头节点加入到 HazardPointers 中 [zhuyb 2024-08-21 11:09:44]
            pHazardPointers1->store(m_Head, std::memory_order_release);
            TaggedNodePtr head = pHazardPointers1->load(std::memory_order_acquire);
 
            // 判断头节点是否变更 [zhuyb 2025-02-08 15:02:20]
            if (m_Head.load(std::memory_order_acquire) != head)
            {
                // std::cout << "aaaa" << std::endl;
                std::this_thread::yield();
                continue;
            }

            TaggedNodePtr tail = m_Tail.load(std::memory_order_acquire);

            // if(head.pNode == tail.pNode)
            if(head == tail)    // 头尾节点相同 [zhuyb 2025-02-08 15:02:20]
            {
                break;
            }
   
            TaggedNodePtr next = head.pNode->next.load(std::memory_order_acquire);
            pHazardPointers2->store(head.pNode->next, std::memory_order_release);

            // 头节点中 next 不存在， 并且head != tail 重新获取头节点 [zhuyb 2025-02-08 15:06:03]
            if (next.pNode == nullptr)
            {
                break;
            }

            // 推进头节点 并记录返回值 [zhuyb 2025-02-08 15:08:22]
            TaggedNodePtr newHead(next.pNode, head.u32Tag + 1);
            if (m_Head.compare_exchange_weak(head, newHead) == true)
            {
                pRetValue = next.pNode->pValue;
                nodeToDelete = head;

                break;
            }
        }

        // 清空 HazardPointers [zhuyb 2025-02-08 15:08:41]
        pHazardPointers1->store(TaggedNodePtr(),std::memory_order_relaxed);
        pHazardPointers2->store(TaggedNodePtr(),std::memory_order_relaxed);
        
        // 读取数据并释放节点 [zhuyb 2025-02-08 15:08:41]
        if(pRetValue != nullptr)
        {
            std::tie(args...) = *(pRetValue);
            delete pRetValue;

            SafeDeleteHazardPointers(nodeToDelete);
            m_u32Count--;

            return true;
        }
        return false;
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
        TaggedNodePtr head = m_Head.load(std::memory_order_acquire);
        Node* pHeadNode = head.pNode;

        TaggedNodePtr tail = m_Tail.load(std::memory_order_acquire);
        Node* pTailNode = tail.pNode;
        if (head == tail)
        {
            std::cout << __LINE__ << " " << "tag:" << tail.u32Tag << " Node: " << tail.pNode->next.load().pNode << std::endl;
            std::cout << __LINE__ << " " << "catch:read " << m_u32Count.load() << ":" << n32ReadCount << std::endl;
            return;
        }

        TaggedNodePtr curl = head;

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
    bool GetHazardPointers(std::atomic<TaggedNodePtr>*& pHazardPointers1, std::atomic<TaggedNodePtr>*& pHazardPointers2)
    {
        if(s_pHazardPointers1 == nullptr || s_pHazardPointers2 == nullptr)
        {

            uint64_t u64ThreadID = Arrow::Other::GetThreadID();

            if (m_u32HazardPointerIndex.load() >= (int32_t)m_arrHazardPointers.size())
            {
                std::cout << __LINE__ << " " << "There are too many threads, exceeding the upper limit of " << m_arrHazardPointers.size() << std::endl;
                return false;
            }
            std::lock_guard<std::mutex> lock(m_mutexMapHazardPointers);
            auto it = m_mapHazardPointers.find(u64ThreadID);
            if (it == m_mapHazardPointers.end())
            {
                m_mapHazardPointers[u64ThreadID] = m_u32HazardPointerIndex.load();
                m_u32HazardPointerIndex++;
            }
            s_pHazardPointers1 = &m_arrHazardPointers[m_mapHazardPointers[u64ThreadID] * HAZARD_POINTERS_COUNT];
            s_pHazardPointers2 = s_pHazardPointers1++;
        }
        pHazardPointers1 = s_pHazardPointers1;
        pHazardPointers2 = s_pHazardPointers2;
        return true;
    }

    void SafeDeleteHazardPointers(const TaggedNodePtr& taggedNodePtr)
    {
        std::vector<TaggedNodePtr> vecDeletedNodes;
        std::vector<TaggedNodePtr> vecNotDeletedNodes;
        std::vector<TaggedNodePtr> vecHPNodes;

        // 缓存需要释放的节点，减少自旋 [zhuyb 2025-02-08 15:09:27]

        m_mutexListDeletedNodes.lock();
        m_ListDeletedNodes.push_back(taggedNodePtr);
        if (m_ListDeletedNodes.size() < 128)
        {
            m_mutexListDeletedNodes.unlock();
            return;
        }
        vecDeletedNodes = m_ListDeletedNodes;
        m_ListDeletedNodes.clear();
        m_mutexListDeletedNodes.unlock();

        for (auto it = m_arrHazardPointers.begin(); it != m_arrHazardPointers.end(); it++)
        {
            TaggedNodePtr taggedNode = it->load();
            if (taggedNode.pNode == nullptr)
            {
                continue;
            }
            vecHPNodes.push_back(taggedNode);
            // auto itIsDel = std::find(vecDeletedNodes.begin(), vecDeletedNodes.end(), taggedNode);
            // if (itIsDel != vecDeletedNodes.end())
            // {
            //     vecDeletedNodes.erase(itIsDel);
            //     vecNotDeletedNodes.push_back(taggedNode);
            // }
        }

        for(auto it = vecHPNodes.begin(); it != vecHPNodes.end(); ++it)
        {
            auto itIsDel = std::find(vecDeletedNodes.begin(), vecDeletedNodes.end(), *it);
            if (itIsDel != vecDeletedNodes.end())
            {
                vecDeletedNodes.erase(itIsDel);
                vecNotDeletedNodes.push_back(*it);
            }
        }

        for(auto it = vecDeletedNodes.begin(); it != vecDeletedNodes.end(); it++)
        {
            delete it->pNode;
            m_u64DeleteCount++;
        }

        if(vecNotDeletedNodes.size() > 0)
        {
            m_mutexListDeletedNodes.lock();
            m_ListDeletedNodes.insert(m_ListDeletedNodes.end(), vecNotDeletedNodes.begin(), vecNotDeletedNodes.end());
            m_mutexListDeletedNodes.unlock();
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
    alignas(64) std::atomic<TaggedNodePtr> m_Head;    // 链表头部 [zhuyb 2024-08-21 11:09:07]
    alignas(64) std::atomic<TaggedNodePtr> m_Tail;    // 链表尾部 [zhuyb 2024-08-21 11:09:24]
    std::atomic<uint32_t> m_u32Count{0};    // 链表个数 [zhuyb 2024-08-21 11:09:30]
    std::string m_strQueueName;
    
    // Hazard Pointer 记录 Push线程用一个，Pop线程用两个，统一每个线程用2个 [zhuyb 2025-02-06 16:29:51]
    std::array<std::atomic<TaggedNodePtr>, MaxThreadCount * HAZARD_POINTERS_COUNT> m_arrHazardPointers;
    std::mutex m_mutexMapHazardPointers;
    std::atomic<int32_t> m_u32HazardPointerIndex{0};
    std::map<uint64_t, int32_t> m_mapHazardPointers;
    thread_local static std::atomic<TaggedNodePtr>* s_pHazardPointers1;
    thread_local static std::atomic<TaggedNodePtr>* s_pHazardPointers2;

    std::mutex m_mutexListDeletedNodes;
    std::vector<TaggedNodePtr> m_ListDeletedNodes;    // 缓存需要释放的节点，减少自旋 [zhuyb 2025-02-06 16:30:29]
    thread_local static std::vector<TaggedNodePtr> s_LocalDeletedCache;

    // test [zhuyb 2025-02-06 15:34:42]
    std::atomic<uint64_t> m_u64Yield1{0};
    std::atomic<uint64_t> m_u64Yield2{0};

    std::atomic<uint64_t> m_u64CreateCount{0};  // 创建节点数 [zhuyb 2025-02-06 15:34:50]
    std::atomic<uint64_t> m_u64DeleteCount{0};  // 删除节点数 [zhuyb 2025-02-06 15:34:56]
};

template<int32_t MaxThreadCount, int32_t MaxDeleteNodeCacheCount, typename ...Args>
thread_local std::atomic<typename LockFreeQueueV2<MaxThreadCount, MaxDeleteNodeCacheCount, Args...>::TaggedNodePtr>* LockFreeQueueV2<MaxThreadCount, MaxDeleteNodeCacheCount, Args...>::s_pHazardPointers1 = nullptr;

template<int32_t MaxThreadCount, int32_t MaxDeleteNodeCacheCount, typename ...Args>
thread_local std::atomic<typename LockFreeQueueV2<MaxThreadCount, MaxDeleteNodeCacheCount, Args...>::TaggedNodePtr>* LockFreeQueueV2<MaxThreadCount, MaxDeleteNodeCacheCount, Args...>::s_pHazardPointers2 = nullptr;

template<int32_t MaxThreadCount, int32_t MaxDeleteNodeCacheCount, typename ...Args>
thread_local std::vector<typename LockFreeQueueV2<MaxThreadCount, MaxDeleteNodeCacheCount, Args...>::TaggedNodePtr> LockFreeQueueV2<MaxThreadCount, MaxDeleteNodeCacheCount, Args...>::s_LocalDeletedCache;


template<typename... Args>
using LockFreeQueue_32T_1C = LockFreeQueueV2<32, 1, Args...>;

template<typename... Args>
using LockFreeQueue_32T_5C = LockFreeQueueV2<32, 5, Args...>;

}
}
