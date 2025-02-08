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
4.SPMC模式 x86_64:测试通过 ARM64:未测试 ARM32:未测试
5.MPMC模式 x86_64:测试通过 ARM64:未测试 ARM32:未测试
6.性能测试 x86_64:未测试 ARM64:未测试 ARM32:未测试
*/

// 无锁队列，无锁队列必须有一个哑结点。否则在处理插入第一个节点和最后一个节点的时候会出现重入问题 [zhuyb 2023-10-15 17:24:01]
template<typename ...Args>
class LockFreeQueueMPSC
{
    using Local = LockFreeQueueMPSC<Args...>;
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
    LockFreeQueueMPSC()
    {
        m_u64CreateCount++;
        Node* pDummyNode = new Node(nullptr);
        TaggedNodePtr taggedDummyNode(pDummyNode, 0);
        m_Head.store(taggedDummyNode, std::memory_order_relaxed);
        m_Tail.store(taggedDummyNode, std::memory_order_relaxed);
        m_strQueueName = "LockFreeQueueMPSC";
    }
    LockFreeQueueMPSC(const Local&) = delete;

    virtual ~LockFreeQueueMPSC()
    {
        // 清空队里所有节点 [zhuyb 2025-02-08 14:14:28]
        while (CallPopHelper<Args...>())
        {
            
        }

        // 删除哑结点 [zhuyb 2025-02-08 14:14:28]
        delete m_Tail.load().pNode;
        m_u64DeleteCount++;

        // 打印统计信息 [zhuyb 2025-02-08 14:14:54]
        std::cout << __LINE__ << " " << "LockFreeQueueMPSC Create:Delete:Diff "
                  << m_u64CreateCount.load() << ":"
                  << m_u64DeleteCount.load() << ":"
                  << m_u64CreateCount.load() - m_u64DeleteCount.load() << std::endl;
    }

    bool Push(Args... args)
    {
        // 创建一个新节点 [zhuyb 2024-08-21 11:09:44]
        m_u64CreateCount++;
        Data* pNewValue = new Data(std::forward<Args>(args)...);
        Node* pNewNode = new Node(pNewValue);
        
        for(;;)
        {
            // 获取尾节点 并将尾节点加入到 HazardPointers 中 [zhuyb 2024-08-21 11:09:44]
            TaggedNodePtr tail = m_Tail.load(std::memory_order_acquire);
            // 判断尾节点是否变更，保证 HazardPointers 中爆出你的尾节点为有效的 [zhuyb 2025-02-08 14:25:11]
            if(m_Tail.load(std::memory_order_relaxed) != tail)
            {
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
        Data* pRetValue = nullptr;
        TaggedNodePtr nodeToDelete;
        for (;;)
        {
            //初始化数据
            pRetValue = nullptr;

            // 获取头节点 并将头节点加入到 HazardPointers 中 [zhuyb 2024-08-21 11:09:44]
            TaggedNodePtr head = m_Head.load(std::memory_order_acquire);
            // 判断头节点是否变更 [zhuyb 2025-02-08 15:02:20]
            if (m_Head.load(std::memory_order_relaxed) != head)
            {
                continue;
            }

            TaggedNodePtr next = head.pNode->next.load(std::memory_order_acquire);
            TaggedNodePtr tail = m_Tail.load(std::memory_order_acquire);
            // if(head.pNode == tail.pNode)
            if(head == tail)    // 头尾节点相同 [zhuyb 2025-02-08 15:02:20]
            {
                if(next.pNode == nullptr)   // 队列为空 [zhuyb 2025-02-08 15:06:03]
                {
                    break;
                }

                // 尾节点中 存在next 推进尾节点 [zhuyb 2025-02-08 15:06:03]
                TaggedNodePtr newTail(next.pNode, head.u32Tag + 1);
                m_Tail.compare_exchange_strong(tail, newTail);
                continue;
            }

            // 头节点中 next 不存在， 并且head != tail 重新获取头节点 [zhuyb 2025-02-08 15:06:03]
            if (next.pNode == nullptr)
            {
                continue;
            }

            pRetValue = next.pNode->pValue;

            // 推进头节点 并记录返回值 [zhuyb 2025-02-08 15:08:22]
            TaggedNodePtr newHead(next.pNode, head.u32Tag + 1);
            if (m_Head.compare_exchange_weak(head, newHead) == true)
            {
                delete head.pNode;
                m_u64DeleteCount++;
                m_u32Count--;
                break;
            }
        }

        // 读取数据并释放节点 [zhuyb 2025-02-08 15:08:41]
        if(pRetValue != nullptr)
        {
            std::tie(args...) = *(pRetValue);
            delete pRetValue;
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

    // test [zhuyb 2025-02-06 15:34:42]
    std::atomic<uint64_t> m_u64Yield1{0};
    std::atomic<uint64_t> m_u64Yield2{0};

    std::atomic<uint64_t> m_u64CreateCount{0};  // 创建节点数 [zhuyb 2025-02-06 15:34:50]
    std::atomic<uint64_t> m_u64DeleteCount{0};  // 删除节点数 [zhuyb 2025-02-06 15:34:56]
};

}
}
