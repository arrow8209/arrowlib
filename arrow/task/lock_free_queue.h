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
#include "arrow/other/std_assist.h"

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
        Data* pNewValue = new Data(std::forward<Args>(args)...);
        Node* pNewNode = new Node(pNewValue);
        Node* pOldTail = nullptr;
        Node* pNullNode = nullptr;
        do
        {
            pOldTail = m_pTail.load();
        } while (!pOldTail->pNext.compare_exchange_weak(pNullNode, pNewNode) != true);

        m_pTail.compare_exchange_weak(pOldTail, pNewNode);
        m_u32Count++;

        return true;
    }

    bool Pop(Args&... args)
    {
        Node* pDummyHead = m_pHead.load();
        Node* pReadHead = nullptr;
        Data* pRetValue = nullptr;
        do
        {
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
        } while (m_pHead.compare_exchange_weak(pDummyHead, pReadHead) != true);

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
private:
    std::atomic<Node*> m_pHead{nullptr};
    std::atomic<Node*> m_pTail{nullptr};
    std::atomic<uint32_t> m_u32Count{0};
};

}
}
