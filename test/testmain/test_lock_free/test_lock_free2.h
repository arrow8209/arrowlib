#pragma once
#include <map>
#include <chrono>
#include "arrow/other/std_assist.h"
#include "arrow/task/lock_free_queue.h"
#include "arrow/task/lock_free_queue2.h"

class CTestLockFree2
{
public:
    CTestLockFree2()
    {
    }
    virtual ~CTestLockFree2()
    {
    }

public:
    void Start(int n32PushThreadCount, int n32PopThreadCount)
    {
        m_bRun = true;
        for(int i = 0; i < n32PopThreadCount; i++)
        {
            m_vecPopThreads.emplace_back(&CTestLockFree2::PopThread, this);
        }
        for(int i = 0; i < n32PushThreadCount; i++)
        {
            m_vecPushThreads.emplace_back(&CTestLockFree2::PushThread, this);
        }

        m_thCheckTime = std::thread(&CTestLockFree2::CheckTime, this);
    }

    void Stop()
    {
        m_bRun = false;

        if(m_thCheckTime.joinable())
            m_thCheckTime.join();

        for (auto& th : m_vecPushThreads)
        {
            if(th.joinable())
                th.join();
        }

        for (auto& th : m_vecPopThreads)
        {
            if(th.joinable())
                th.join();
        }

        // uint64_t u64PopThreadID = 0;
        // uint64_t u64PopIndex = 0;

        // std::cout << __LINE__ << " " << "queue count: " << m_LockFreeQueue.Size() << std::endl;

        // while(true)
        // {
        //     if(m_LockFreeQueue.Size() < 5)
        //         break;
        //     m_tpLastPushTime = std::chrono::steady_clock::now();
        //     if(!m_LockFreeQueue.Pop(u64PopThreadID, u64PopIndex))
        //     {
        //         break;
        //     }
        //     CheckMPSC(u64PopThreadID, u64PopIndex);
        // }
        
        // std::cout << __LINE__ << " " << "queue count: " << m_LockFreeQueue.Size() << std::endl;
        // m_LockFreeQueue.TestPrintQueue();
    }

    void PushThread()
    {
        uint64_t u64ThreadID = Arrow::Other::GetThreadID();
        // std::cout << "PushThread: " << u64ThreadID << std::endl;
        uint64_t u64Index = 0;
        while(m_bRun)
        {
            m_tpLastPopTime = std::chrono::steady_clock::now();
            if(m_LockFreeQueue.Push(u64ThreadID, u64Index) == false)
            {
                std::cout << "Push failed: " << u64ThreadID << " " << u64Index << std::endl;
            }
            u64Index++;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void PopThread()
    {
        uint64_t u64ThreadID = Arrow::Other::GetThreadID();
        // std::cout << "PopThread: " << u64ThreadID << std::endl;
        uint64_t u64PopThreadID = 0;
        uint64_t u64PopIndex = 0;
        while(m_bRun)
        {
            m_tpLastPushTime = std::chrono::steady_clock::now();
            if(!m_LockFreeQueue.Pop(u64PopThreadID, u64PopIndex))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }
            CheckMPSC(u64PopThreadID, u64PopIndex);
        }
    }

    void CheckTime()
    {
        while(m_bRun)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::chrono::steady_clock::time_point tpNow = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> duration = tpNow - m_tpLastPopTime;
            if (duration.count() > 100)
            {
                std::cout << "Pop time too long: " << duration.count() << std::endl;
                m_bRun = false;
            }
            duration = tpNow - m_tpLastPushTime;
            if (duration.count() > 100)
            {
                std::cout << "Push time too long: " << duration.count() << std::endl;
                m_bRun = false;
            }
        }
    }
private:
    void CheckMPSC(uint64_t u64ThreadID, uint64_t u64NewIndex)
    {
        if(mapThreadToLastIndex.find(u64ThreadID) == mapThreadToLastIndex.end())
        {
            if(u64NewIndex == 0)
            {
                mapThreadToLastIndex[u64ThreadID] = 0;
                return;
            }
            std::cout << "MPSC Error: ThreadID: " << u64ThreadID << " NewIndex: " << u64NewIndex  << std::endl;
            return;
        }
        
        if(mapThreadToLastIndex[u64ThreadID] + 1 == u64NewIndex)
        {
            mapThreadToLastIndex[u64ThreadID] = u64NewIndex;
            return;
        }
        else
        {
            std::cout << "MPSC Error: ThreadID: " << u64ThreadID << " NewIndex: " << u64NewIndex << " LastIndex: " << mapThreadToLastIndex[u64ThreadID] << std::endl;
            return;
        }
    }
private:
    bool m_bRun = true;
    Arrow::Pattern::LockFreeQueue2<uint64_t, uint64_t> m_LockFreeQueue;
    std::vector<std::thread> m_vecPopThreads;
    std::vector<std::thread> m_vecPushThreads;
    std::thread m_thCheckTime;
    std::chrono::steady_clock::time_point m_tpLastPopTime;
    std::chrono::steady_clock::time_point m_tpLastPushTime;
    // FILE* m_File = nullptr;

    std::map<uint64_t, uint64_t> mapThreadToLastIndex;
};


void TestLockFree2A()
{
    int* pTmp = new int(10);
    Arrow::Pattern::LockFreeQueue2<int, float> lockFreeQueue;
    lockFreeQueue.Push((int)1, float(1.1));
    lockFreeQueue.Push((int)2, float(1.2));
    // lockFreeQueue.Push((int)3, float(1.3));
    // lockFreeQueue.Push((int)4, float(1.4));
    // lockFreeQueue.Push((int)5, float(1.5));

    int n1 = 0;
    float f1 = 0;
    bool bRet = true;
    bRet = lockFreeQueue.Pop(n1, f1);
    bRet = lockFreeQueue.Pop(n1, f1);
    bRet = lockFreeQueue.Pop(n1, f1);
    // bRet = lockFreeQueue.Pop(n1, f1);
    // bRet = lockFreeQueue.Pop(n1, f1);
    // bRet = lockFreeQueue.Pop(n1, f1);
    // bRet = lockFreeQueue.Pop(n1, f1);

    // std::cout << lockFreeQueue.Size() << std::endl;
    // std::cout << lockFreeQueue.Empty() << std::endl;
    lockFreeQueue.TestPrintQueue();
}

void TestLockFree2B()
{
    for (size_t i = 0; i < 6; i++)
    {
        std::cout << "============================= " << i << " =============================" << std::endl;
        CTestLockFree2 testLockFree2;
        testLockFree2.Start(10, 1);
        std::string strMsg = "Hello, world!";
        std::this_thread::sleep_for(std::chrono::seconds(10));
        testLockFree2.Stop();
    }
}