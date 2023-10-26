#pragma once
#include <thread>
#include <chrono>
#include "arrow/arrow.h"

#define RUNTIMES 1000
#define THREADS 10

class CTestTask3 : public Arrow::Pattern::TaskOneThread2<CTestTask3>
{
public :
    CTestTask3() {}
    virtual ~CTestTask3()
    {
        Arrow::Pattern::TaskOneThread2<CTestTask3>::Stop();
        std::cout << "RumTime:" << m_nCount << " Catch:" << TaskCount() << std::endl;
    }

public:
    void Push(int index)
    {
        AddTask(&CTestTask3::Task1, &CTestTask3::Clear1, index);
    }

    void Test()
    {
        for (int i = 0; i < RUNTIMES; i++)
        {
            AddTask(&CTestTask3::Task1, &CTestTask3::Clear1, i);
        }
    }

    void Task1(int index)
    {
        m_nCount++;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    void Clear1(int nIndex)
    {
        m_nCount++;
    }

protected:
    int32_t m_nCount = 0;

};

template <typename T>
void ThreadInput3(T* pTest)
{
    pTest->Test();
}

void TestTak3()
{
    {
        CTestTask3 task;
        task.Activate();
        std::vector<std::thread> vThread;
        for (int i = 0; i < THREADS; i++)
        {
            vThread.push_back(std::thread(ThreadInput3<CTestTask3>, &task));
        }
        for (int i = 0; i < THREADS; i++)
        {
            vThread[i].join();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(THREADS*RUNTIMES / 2));
    }
}