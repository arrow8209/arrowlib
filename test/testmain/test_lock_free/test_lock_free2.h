#pragma once
#include <map>
#include <chrono>
#include "arrow/other/std_assist.h"
#include "arrow/task/lock_free_queue.h"
#include "arrow/task/lock_free_queue2.h"
#include "arrow/task/lock_free_queue_mpsc.h"
// #include "arrow/task/lock_free_queueV3.h"

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
        std::cout << "Start: " << n32PushThreadCount << " " << n32PopThreadCount << std::endl;
        m_bRun = true;
        m_n32PopThreadCount = n32PopThreadCount;
        m_n32PushThreadCount = n32PushThreadCount;
        m_tpLastPushTime = std::chrono::steady_clock::now();
        m_tpLastPopTime = std::chrono::steady_clock::now();

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
            // if(m_LockFreeQueue.Size() > 10000)
            // {
            //     std::this_thread::sleep_for(std::chrono::milliseconds(1));
            // }
            m_tpLastPushTime = std::chrono::steady_clock::now();
            m_u64PushCount++;
            if(m_LockFreeQueue.Push(u64ThreadID, u64Index) == false)
            {
                std::cout << "Push failed: " << u64ThreadID << " " << u64Index << std::endl;
            }
            u64Index++;
            // std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    void PopThread()
    {
        uint64_t u64PopThreadID = Arrow::Other::GetThreadID();
        // std::cout << "PopThread: " << u64ThreadID << std::endl;
        uint64_t u64ThreadID = 0;
        uint64_t u64Index = 0;
        while(m_bRun)
        {
            m_tpLastPopTime = std::chrono::steady_clock::now();
            if(!m_LockFreeQueue.Pop(u64ThreadID, u64Index))
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                continue;
            }
            m_u64PopCount++;
            if(m_n32PushThreadCount <= 1 && m_n32PopThreadCount <= 1)
            {
                CheckMPSC(u64ThreadID, u64Index);
                CheckSPMC(u64PopThreadID, u64ThreadID, u64Index);
            }
            else if(m_n32PushThreadCount > 1 && m_n32PopThreadCount <= 1)
            {
                CheckMPSC(u64ThreadID, u64Index);
            }
            else if(m_n32PushThreadCount <= 1 && m_n32PopThreadCount > 1)
            {
                CheckSPMC(u64PopThreadID, u64ThreadID, u64Index);
            }
            else if(m_n32PushThreadCount > 1 && m_n32PopThreadCount > 1)
            {
                CheckMPMC(u64PopThreadID, u64ThreadID, u64Index);
            }
        }
    }

    void CheckTime()
    {
        std::chrono::steady_clock::time_point tpPrintCacheTime = std::chrono::steady_clock::now();
        uint64_t u64PushCount = 0;
        uint64_t u64PopCount = 0;
        while(m_bRun)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            std::chrono::steady_clock::time_point tpNow = std::chrono::steady_clock::now();
            std::chrono::duration<double, std::milli> duration = tpNow - m_tpLastPopTime;
            if (duration.count() > 100)
            {
                std::cout << "Pop time too long: " << duration.count() << std::endl;
                // m_bRun = false;
            }
            duration = tpNow - m_tpLastPushTime;
            if (duration.count() > 100)
            {
                std::cout << "Push time too long: " << duration.count() << std::endl;
                // m_bRun = false;
            }
            duration = tpNow - tpPrintCacheTime;
            if(duration.count() > 1000)
            {
                std::cout << "in:out:cache " << m_u64PushCount.load() << ":" << m_u64PopCount.load() << ":" << m_LockFreeQueue.Size()
                    << " run in:out " << m_u64PushCount.load() - u64PushCount << ":" << m_u64PopCount.load() - u64PopCount << std::endl;

                u64PushCount = m_u64PushCount.load();
                u64PopCount = m_u64PopCount.load();
                
                tpPrintCacheTime = tpNow;
                if(m_n32PushThreadCount <= 1 && m_n32PopThreadCount > 1)
                {
                    CheckSPMC_Timer();
                }
                else if(m_n32PushThreadCount > 1 && m_n32PopThreadCount > 1)
                {
                    CheckMPMC_Timer();
                }
            }
        }
    }
private:
    void CheckMPSC(uint64_t u64ThreadID, uint64_t u64NewIndex)
    {
        if(m_mapThreadToLastIndex.find(u64ThreadID) == m_mapThreadToLastIndex.end())
        {
            if(u64NewIndex == 0)
            {
                m_mapThreadToLastIndex[u64ThreadID] = 0;
                return;
            }
            std::cout << "MPSC Error: ThreadID: " << u64ThreadID << " NewIndex: " << u64NewIndex  << std::endl;
            return;
        }
        
        if(m_mapThreadToLastIndex[u64ThreadID] + 1 == u64NewIndex)
        {
            m_mapThreadToLastIndex[u64ThreadID] = u64NewIndex;
            return;
        }
        else
        {
            std::cout << "MPSC Error: ThreadID: " << u64ThreadID << " NewIndex: " << u64NewIndex << " LastIndex: " << m_mapThreadToLastIndex[u64ThreadID] << std::endl;
            return;
        }
    }

    void CheckSPMC(uint64_t u64PopThread, uint64_t u64PushThreadID, uint64_t u64NewIndex)
    {
        // uint64_t u64CacheCount = m_u64PushCount.load() - m_u64PopCount.load();
        // if(u64CacheCount >= 3)
        // {
        //     std::cout << "SPMC WARN: m_u64PushCount:m_u64PopCount:diff " 
        //         << m_u64PushCount.load() << ":" << m_u64PopCount.load() << ":" << u64CacheCount << std::endl;
        // }

        m_mutexSPMC.lock();
        m_vecRecvIndex.push_back(u64NewIndex);
        m_mutexSPMC.unlock();

    }

    void CheckSPMC_Timer()
    {
        m_mutexSPMC.lock();
        m_vecTmpRecvIndex.insert(m_vecTmpRecvIndex.end(), m_vecRecvIndex.begin(), m_vecRecvIndex.end());
        m_vecRecvIndex.clear();
        m_mutexSPMC.unlock();

        std::sort(m_vecTmpRecvIndex.begin(), m_vecTmpRecvIndex.end());
        // 找到第一个不连续的位置
        auto it = m_vecTmpRecvIndex.begin();
        while (it + 1 != m_vecTmpRecvIndex.end() && *(it + 1) == *it + 1)
        {
            ++it;
        }

        // 如果找到了不连续的位置，删除之前的所有元素
        if (it != m_vecTmpRecvIndex.begin())
        {
            m_vecTmpRecvIndex.erase(m_vecTmpRecvIndex.begin(), it);
        }

        std::cout << "SPMC TmpRecvIndex size:" << m_vecTmpRecvIndex.size() << " value: ";
        for (auto& index : m_vecTmpRecvIndex)
        {
            std::cout << index << " ";
        }
        std::cout << std::endl;
    }

    void CheckMPMC(uint64_t u64PopThread, uint64_t u64PushThreadID, uint64_t u64NewIndex)
    {
        // uint64_t u64CacheCount = m_u64PushCount.load() - m_u64PopCount.load();
        // if (u64CacheCount >= (m_n32PushThreadCount * 4))
        // {
        //     std::cout << "MPMC WARN: m_u64PushCount:m_u64PopCount:diff "
        //               << m_u64PushCount.load() << ":" << m_u64PopCount.load() << ":" << u64CacheCount << std::endl;
        // }

        m_mutexMPMC.lock();
        m_mapRecvIndex[u64PushThreadID].push_back(u64NewIndex);
        m_mutexMPMC.unlock();
    }

    void CheckMPMC_Timer()
    {
        m_mutexMPMC.lock();
        for(auto& pair : m_mapRecvIndex)
        {
            m_mapTmpRecvIndex[pair.first].insert(m_mapTmpRecvIndex[pair.first].end(), pair.second.begin(), pair.second.end());
            pair.second.clear();
        }
        m_mutexMPMC.unlock();

        for (auto& pair : m_mapTmpRecvIndex)
        {
            std::sort(pair.second.begin(), pair.second.end());
            // 找到第一个不连续的位置
            auto it = pair.second.begin();
            while (it + 1 != pair.second.end() && *(it + 1) == *it + 1)
            {
                ++it;
            }

            // 如果找到了不连续的位置，删除之前的所有元素
            if (it != pair.second.begin())
            {
                pair.second.erase(pair.second.begin(), it);
            }

            // std::cout << "MPMC TmpRecvIndex ThreadID:" << pair.first << " size:" << pair.second.size() << " value: ";
            // for (auto& index : pair.second)
            // {
            //     std::cout << index << " ";
            // }
            // std::cout << std::endl;

            std::cout << "MPMC TmpRecvIndex ThreadID:" << pair.first << " size:" << pair.second.size() << std::endl;
        }
    }
private:
    bool m_bRun = true;
    Arrow::Pattern::LockFreeQueueV2<32, 32, uint64_t, uint64_t> m_LockFreeQueue;
    // Arrow::Pattern::LockFreeQueueMPSC<uint64_t, uint64_t> m_LockFreeQueue;
    // Arrow::Pattern::LockFreeQueueV3<uint64_t, uint64_t> m_LockFreeQueue;
    // Arrow::Pattern::SimpleLockQueue<uint64_t, uint64_t> m_LockFreeQueue;
    
    std::vector<std::thread> m_vecPopThreads;
    std::vector<std::thread> m_vecPushThreads;

    int32_t m_n32PopThreadCount = 0;
    int32_t m_n32PushThreadCount = 0;

    std::atomic<uint64_t> m_u64PushCount{0};
    std::atomic<uint64_t> m_u64PopCount{0};

    std::thread m_thCheckTime;
    std::chrono::steady_clock::time_point m_tpLastPopTime;
    std::chrono::steady_clock::time_point m_tpLastPushTime;

    // for mpsc check
    std::map<uint64_t, uint64_t> m_mapThreadToLastIndex;

    // for spmc check
    std::mutex m_mutexSPMC;
    std::vector<uint64_t> m_vecRecvIndex;
    std::vector<uint64_t> m_vecTmpRecvIndex;

    // for mpmc check
    std::mutex m_mutexMPMC;
    std::map<uint64_t, std::vector<uint64_t>> m_mapRecvIndex;
    std::map<uint64_t, std::vector<uint64_t>> m_mapTmpRecvIndex;
};


void TestLockFree2A()
{
    int* pTmp = new int(10);
    Arrow::Pattern::LockFreeQueue_32T_1C<int, float> lockFreeQueue;
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
    std::cout << "==========================================================" << std::endl;
    CTestLockFree2 testLockFree2;
    testLockFree2.Start(8, 1);
    std::string strMsg = "Hello, world!";
    std::cin >> strMsg;
    testLockFree2.Stop();

    // for (size_t i = 0; i < 6; i++)
    // {
    //     std::cout << "============================= " << i << " =============================" << std::endl;
    //     CTestLockFree2 testLockFree2;
    //     testLockFree2.Start(1, 10);
    //     std::this_thread::sleep_for(std::chrono::seconds(10));
    //     testLockFree2.Stop();
    // }
}