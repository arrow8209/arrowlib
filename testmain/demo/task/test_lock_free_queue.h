#pragma once
#include <thread>
#include <chrono>
#include "arrow/arrow.h"

#define RUNTIMES 1000
#define THREADS 2


template <typename T>
void ThreadPushLockFree(T* pTest, int nIndex)
{
    for(int i = 0; i < RUNTIMES; i++)
    {
        pTest->Push(nIndex * RUNTIMES + i);
    }
}

void TestLockFree()
{

    while (true)
    {
        Arrow::Pattern::LockFreeQueue<int> lockFreeQueue;

        std::vector<std::thread> vThread;
        for (int i = 0; i < THREADS; i++)
        {
            vThread.push_back(std::thread(std::bind(ThreadPushLockFree<Arrow::Pattern::LockFreeQueue<int>>, &lockFreeQueue, i)));
        }
        for (int i = 0; i < THREADS; i++)
        {
            vThread[i].join();
        }

        int nTmp = 0;
        int nCount = 0;
        while (lockFreeQueue.Empty() == false)
        {
            if (lockFreeQueue.Pop(nTmp) == true)
            {
                nCount++;
            }
        }
        std::cout << "nCount:" << nCount << ":" << lockFreeQueue.Size() << std::endl;
    }
}