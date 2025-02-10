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
class LockQueue
{
    using Local = LockQueue<Args...>;
    using Data = std::tuple<Args...>;

    struct Node
    {
        Data* pValue = nullptr;
        Node* pNext{nullptr};
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
    LockQueue()
    {
        Node* pDummyNode = new Node(nullptr);
        m_pHead = pDummyNode;
        m_pTail = pDummyNode;
        m_strQueueName = "LockQueue";
    }
    LockQueue(const Local&) = delete;

    virtual ~LockQueue()
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

        std::lock_guard<std::mutex> lock(m_mutexQueue);
        m_u32Count++;
        m_pTail->pNext = pNewNode;
        m_pTail = pNewNode;
        
        return true;
    }

    bool Pop(Args&... args)
    {
        if (m_pHead->pNext == nullptr)
            return false;
        
        m_mutexQueue.lock();
        Node* pTmpHead = m_pHead;

        if (pTmpHead->pNext == nullptr)
        {
            m_mutexQueue.unlock();
            return false;
        }

        Data* pRetValue = m_pHead->pNext->pValue;
        m_pHead = m_pHead->pNext;
        m_u32Count--;
        m_mutexQueue.unlock();

        std::tie(args...) = *(pRetValue);
        delete pTmpHead;
        
        return true;
    }

    uint32_t Size()
    {
        // uint32_t u32Size = m_u32InCount - m_u32OutCount;
        // printf("error\n");
        // if(u32Size > 4967295 )
        // {
        //     printf("error\n");
        // }
        // return u32Size;
        return m_u32Count.load();
    }

    bool Empty()
    {
        return m_pHead == m_pTail;
    }

    void SetName(const char* szName)
    {
        m_strQueueName = szName;
    }

private:
    Node* m_pHead = nullptr;    // 链表头部 [zhuyb 2024-08-21 11:09:07]
    Node* m_pTail = nullptr;    // 链表尾部 [zhuyb 2024-08-21 11:09:24]
    std::atomic<uint32_t> m_u32Count{0};
    // uint32_t m_u32InCount{0};    // 链表个数 [zhuyb 2024-08-21 11:09:30]
    // uint32_t m_u32OutCount{0};    // 链表个数 [zhuyb 2024-08-21 11:09:30]
    std::string m_strQueueName;
    std::mutex m_mutexQueue;
};

}
}
