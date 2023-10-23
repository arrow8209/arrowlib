#pragma once
#include <thread>
#include <chrono>
#include "arrow/arrow.h"

#define RUNTIMES 1000000

class CTestTask1 : public Arrow::Pattern::TaskOneThread
{
public :
    CTestTask1(){}
    virtual ~CTestTask1(){
        std::cout << "TaskOneThread " << "Input:" << std::chrono::duration_cast<std::chrono::microseconds>(m_tpInput2 - m_tpInput1).count() << " "
                    << "Output:" << std::chrono::duration_cast<std::chrono::microseconds>(m_tpRun2 - m_tpRun1).count() << std::endl;
    }

public:
    void Push(int index)
    {
        AddTask<CTestTask1>(&CTestTask1::Task1, &CTestTask1::Clear1, index);
    }

    void Test()
    {
        m_tpInput1 = std::chrono::steady_clock::now();
        for (int i = 0; i < RUNTIMES; i++)
        {
            AddTask<CTestTask1>(&CTestTask1::Task1, &CTestTask1::Clear1, i);
        }
        m_tpInput2 = std::chrono::steady_clock::now();
        // if(m_bIsRun.load() == true)
        // {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        // }
    }

    void Task1(int index)
    {
        if (index == 0)
        {
            m_tpRun1 =std::chrono::steady_clock::now();
            return;
        }

        if(index == RUNTIMES - 1)
        {
            m_tpRun2 = std::chrono::steady_clock::now();
            StopFlag();
        }
    }
    void Clear1(int nIndex)
    {
        assert(false);
    }

protected:
    int32_t m_nCount = 0;
    std::chrono::steady_clock::time_point m_tpInput1;
    std::chrono::steady_clock::time_point m_tpInput2;

    std::chrono::steady_clock::time_point m_tpRun1;
    std::chrono::steady_clock::time_point m_tpRun2;
};


class CTestTask2 : public Arrow::Pattern::TaskOneThread2<CTestTask2>
{
public :
    CTestTask2() {}
    virtual ~CTestTask2()
    {
        std::cout << "TaskOneThread2 " << "Input:" << std::chrono::duration_cast<std::chrono::microseconds>(m_tpInput2 - m_tpInput1).count() << " "
                  << "Output:" << std::chrono::duration_cast<std::chrono::microseconds>(m_tpRun2 - m_tpRun1).count() << std::endl;
    }

public:
    void Push(int index)
    {
        AddTask(&CTestTask2::Task1, &CTestTask2::Clear1, index);
    }

    void Test()
    {
        m_tpInput1 = std::chrono::steady_clock::now();
        for (int i = 0; i < RUNTIMES; i++)
        {
            AddTask(&CTestTask2::Task1, &CTestTask2::Clear1, i);
        }
        m_tpInput2 = std::chrono::steady_clock::now();
        // if(m_bIsRun.load() == true)
        // {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        // }
    }

    void Task1(int index)
    {
        if (index == 0)
        {
            m_tpRun1 =std::chrono::steady_clock::now();
            return;
        }

        if(index == RUNTIMES - 1)
        {
            m_tpRun2 = std::chrono::steady_clock::now();
            StopFlag();
        }
    }
    void Clear1(int nIndex)
    {
        assert(false);
    }

protected:
    int32_t m_nCount = 0;
    std::chrono::steady_clock::time_point m_tpInput1;
    std::chrono::steady_clock::time_point m_tpInput2;

    std::chrono::steady_clock::time_point m_tpRun1;
    std::chrono::steady_clock::time_point m_tpRun2;
};

// template<typename T>
// void ThreadInput(T* pTest, int nCount, int nIndex)
// {
//     auto tp1 = std::chrono::steady_clock::now();
//     for (int i = 0; i < 1000000; i++)
//     {
//         pTest->Push(i * nCount + nIndex);
//     }
//     auto tp2 = std::chrono::steady_clock::now();
//     std::cout << "Input:" << nIndex << ":" << std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count() << std::endl;

// }

void TestTaskPerformance()
{
    int nInputThread = 10;

    for (int i = 0; i < 10; i++)
    {
        CTestTask1 task;
        // CTestTask2 task;
        task.Activate();
        task.Test();
    }
}