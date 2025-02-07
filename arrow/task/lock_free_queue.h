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

// 无锁队列，无锁队列必须有一个哑结点。否则在处理插入第一个节点和最后一个节点的时候会出现重入问题 [zhuyb 2023-10-15 17:24:01]
template<typename ...Args>
class LockFreeQueue
{
    using Local = LockFreeQueue<Args...>;
    using Data = std::tuple<Args...>;

    struct Node
    {
        Data* pValue = nullptr;
        std::atomic<Node*> pNext{nullptr};
        Node(Data* pVal) : pValue(pVal), pNext(nullptr)
        {
        }

        ~Node()
        {
            if (pValue != nullptr)
                delete pValue;
        }
    };

public:
    LockFreeQueue()
    {
        Node* pDummyNode = new Node(nullptr);
        m_pHead = pDummyNode;
        m_pTail = pDummyNode;
        m_strQueueName = "LockFreeQueue";
    }
    LockFreeQueue(const Local&) = delete;

    virtual ~LockFreeQueue()
    {
        Node* pCurl = m_pHead;
        while(pCurl !=  nullptr)
        {
            Node* pNext = pCurl->pNext;
            delete pCurl;
            pCurl = pNext;
        }
    }

    bool Push(Args... args)
    {
        // 创建一个新节点 [zhuyb 2024-08-21 11:09:44]
        Data* pNewValue = new Data(std::forward<Args>(args)...);
        Node* pNewNode = new Node(pNewValue);

        Node* pOldTail = nullptr;
        Node* pNullNode = nullptr;
        m_u32Count++;

        do
        {
            pOldTail = m_pTail.load();
            pNullNode = nullptr;
        } while (pOldTail->pNext.compare_exchange_weak(pNullNode, pNewNode) != true);

        Node* pTmpTail = pOldTail;
        do
        {
            pOldTail = pTmpTail;
        }while(m_pTail.compare_exchange_weak(pOldTail, pNewNode) != true);
        
        return true;
    }

    bool Pop(Args&... args)
    {
        Node* pDummyHead = nullptr;
        Node* pReadHead = nullptr;
        Data* pRetValue = nullptr;
        while(true)
        {
            // 这一句话存在ABA问题，单线程A执行问这一行后暂定，其他线程继续执行会出在112行出现ABA问题。也就是m_pHead==pDummyHead 但是不是同一个节点。 [zhuyb 2025-02-05 08:37:16]
            pDummyHead = m_pHead.load(); 
            
            // 第一个节点是哑结点。真正的第一个是head->next [zhuyb 2023-10-15 15:54:55]
            pReadHead = pDummyHead->pNext.load();
            if (pReadHead == nullptr)
            {
                return false;
            }
            else
            {
                pRetValue = pReadHead->pValue;
            }

            if (m_pHead.compare_exchange_weak(pDummyHead, pReadHead) == true)
            {
                break;
            }
        }

        std::tie(args...) = *(pRetValue);
        delete pDummyHead;
        m_u32Count--;
        return true;
    }

    uint32_t Size()
    {
        return m_u32Count.load();
    }

    bool Empty()
    {
        return m_pHead.load() == m_pTail.load();
    }

    void SetName(const char* szName)
    {
        m_strQueueName = szName;
    }

private:
    std::atomic<Node*> m_pHead{nullptr};    // 链表头部 [zhuyb 2024-08-21 11:09:07]
    std::atomic<Node*> m_pTail{nullptr};    // 链表尾部 [zhuyb 2024-08-21 11:09:24]
    std::atomic<uint32_t> m_u32Count{0};    // 链表个数 [zhuyb 2024-08-21 11:09:30]
    std::string m_strQueueName;
    // std::mutex m_mutexHead;
    // std::mutex m_mutexTail;
};

}
}
