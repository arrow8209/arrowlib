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
class SimpleLockQueue
{
    using Local = SimpleLockQueue<Args...>;
    using Data = std::tuple<Args...>;
    using ListQueue = std::list<Data*>;

public:
    SimpleLockQueue()
    {
        m_strQueueName = "SimpleLockQueue";
    }
    SimpleLockQueue(const Local&) = delete;

    virtual ~SimpleLockQueue()
    {
        for (auto it : m_ListQueue)
        {
            delete it;
        }
        m_ListQueue.clear();
    }

    bool Push(Args... args)
    {
        // 创建一个新节点 [zhuyb 2024-08-21 11:09:44]
        Data* pNewValue = new Data(args...);

        std::lock_guard<std::mutex> lock(m_mutexQueue);
        m_ListQueue.push_back(pNewValue);
        
        return true;
    }

    bool Pop(Args&... args)
    {
        if (m_ListQueue.size() == 0)
            return false;
        
        m_mutexQueue.lock();
        Data* pRetValue = m_ListQueue.front();
        m_ListQueue.pop_front();
        m_mutexQueue.unlock();

        std::tie(args...) = *(pRetValue);
        delete pRetValue;
        
        return true;
    }

    uint32_t Size()
    {
        return m_ListQueue.size();
    }

    bool Empty()
    {
        return m_ListQueue.empty();
    }

    void SetName(const char* szName)
    {
        m_strQueueName = szName;
    }

private:

    ListQueue m_ListQueue;
    std::string m_strQueueName;
    std::mutex m_mutexQueue;
};

}
}
