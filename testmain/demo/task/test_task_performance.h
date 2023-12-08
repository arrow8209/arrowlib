#pragma once
#include <thread>
#include <chrono>
#include <atomic>
#include <iomanip>
#include <inttypes.h>
#include "arrow/arrow.h"

#define RUNTIMES 1000000
#define NUM_THREADS 10

class CTestTask1 : public Arrow::Pattern::TaskOneThread
{
public :
    CTestTask1(){}
    virtual ~CTestTask1(){
        Arrow::Pattern::TaskOneThread::Stop();

        float fInAvgTime = 1.0 * m_u64InputTime.load() / m_u64Count;
        float fOutAvgTime = 1.0 * std::chrono::duration_cast<std::chrono::nanoseconds>(m_tpRun2 - m_tpRun1).count() / m_u64Count;
        printf("CTestTask1 count:%" PRIu64 " InAvgTime:%0.2fns OutAvgTime:%0.2fns\n", m_u64Count, fInAvgTime, fOutAvgTime);
    }

public:
    void Push(int index)
    {
        AddTask<CTestTask1>(&CTestTask1::Task1, &CTestTask1::Clear1, index);
    }

    void Test()
    {
        auto tp1 = std::chrono::steady_clock::now();
        for (int i = 0; i < RUNTIMES; i++)
        {
            AddTask<CTestTask1>(&CTestTask1::Task1, &CTestTask1::Clear1, i);
        }
        auto tp2 = std::chrono::steady_clock::now();
        auto tmSpace = std::chrono::duration_cast<std::chrono::nanoseconds>(tp2 - tp1).count();

        m_u64InputTime.fetch_add(tmSpace);
    }

    void Task1(int index)
    {
        m_u64Count++;
        if (m_u64Count == 1)
        {
            m_tpRun1 = std::chrono::steady_clock::now();
            return;
        }

        if(m_u64Count == RUNTIMES * NUM_THREADS)
        {
            m_tpRun2 = std::chrono::steady_clock::now();
            StopFlag();
        }
    }
    void Clear1(int nIndex)
    {
        m_u64Count++;
        m_tpRun2 = std::chrono::steady_clock::now();
    }

protected:
    uint64_t m_u64Count = 0;
    std::atomic<uint64_t> m_u64InputTime{0};

    std::chrono::steady_clock::time_point m_tpRun1 = std::chrono::steady_clock::time_point::min();
    std::chrono::steady_clock::time_point m_tpRun2 = std::chrono::steady_clock::time_point::min();
};

class CTestTask2 : public Arrow::Pattern::TaskOneThread2<CTestTask2>
{
public :
    CTestTask2() {}
    virtual ~CTestTask2()
    {
        Arrow::Pattern::TaskOneThread2<CTestTask2>::Stop();

        float fInAvgTime = 1.0 * m_u64InputTime.load() / m_u64Count;
        float fOutAvgTime = 1.0 * std::chrono::duration_cast<std::chrono::nanoseconds>(m_tpRun2 - m_tpRun1).count() / m_u64Count;
        printf("CTestTask2 count:%" PRIu64 " InAvgTime:%0.2fns OutAvgTime:%0.2fns\n", m_u64Count, fInAvgTime, fOutAvgTime);
    }

public:
    void Push(int index)
    {
        AddTask(&CTestTask2::Task1, &CTestTask2::Clear1, index);
    }

    void Test()
    {
        auto tp1 = std::chrono::steady_clock::now();
        for (int i = 0; i < RUNTIMES; i++)
        {
            AddTask(&CTestTask2::Task1, &CTestTask2::Clear1, i);
        }
        auto tp2 = std::chrono::steady_clock::now();
        auto tmSpace = std::chrono::duration_cast<std::chrono::nanoseconds>(tp2 - tp1).count();

        m_u64InputTime.fetch_add(tmSpace);
    }

    void Task1(int index)
    {
        m_u64Count++;
        if (m_u64Count == 1)
        {
            m_tpRun1 = std::chrono::steady_clock::now();
            return;
        }

        if(m_u64Count == RUNTIMES * NUM_THREADS)
        {
            m_tpRun2 = std::chrono::steady_clock::now();
            StopFlag();
        }
    }
    void Clear1(int nIndex)
    {
        m_u64Count++;
        m_tpRun2 = std::chrono::steady_clock::now();
    }

protected:
    uint64_t m_u64Count = 0;
    std::atomic<uint64_t> m_u64InputTime{0};

    std::chrono::steady_clock::time_point m_tpRun1 = std::chrono::steady_clock::time_point::min();
    std::chrono::steady_clock::time_point m_tpRun2 = std::chrono::steady_clock::time_point::min();
};

template<typename T>
void ThreadInput(T* pTest)
{
    pTest->Test();
}

template<typename T>
void TestOnePerformance(int nTestCount = 1)
{
    std::cout << "TestOnePerformance Start" << std::endl;

    std::vector<std::thread> vecThread;

    for (int i = 0; i < nTestCount; i++)
    {
        T task;
        task.Activate();
        for(int j = 0; j < NUM_THREADS; j++)
        {
            vecThread.push_back(std::thread(ThreadInput<T>, &task));
        }
        for(int j = 0; j < NUM_THREADS; j++)
        {
            vecThread[j].join();
        }
        while (task.TaskCount() != 0)
            std::this_thread::sleep_for(std::chrono::seconds(1));
        vecThread.clear();
        std::cout << "TestOnePerformance:" << i << std::endl;
    }
    std::cout << "TestOnePerformance End" << std::endl;
}

void TestTaskPerformance()
{
    int32_t nTestCount = 4;
    TestOnePerformance<CTestTask1>(nTestCount);

    TestOnePerformance<CTestTask2>(nTestCount);

    // for (int i = 0; i < 10; i++)
    // {
    //     CTestTask2 task;
    //     task.Activate();
    //     task.Test();
    // }

    // for (int i = 0; i < 10; i++)
    // {
    //     CTestTask3 task;
    //     task.Activate();
    //     task.Test();
    // }
}