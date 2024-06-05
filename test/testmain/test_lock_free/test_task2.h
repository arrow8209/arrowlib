#pragma once
#include <thread>
#include <chrono>
#include "arrow/task/lock_free_queue.h"
#include "arrow/task/task_one_thread2.h"
#include "arrow/task/task_one_thread.h"

class CTestTask1 : public Arrow::Pattern::TaskOneThread2
{
public :
    CTestTask1(){}
    virtual ~CTestTask1(){}

public:
    void Push(int index)
    {
        AddTask<CTestTask1>(&CTestTask1::Task1, &CTestTask1::Clear1, index);
    }

    void Test()
    {
        auto tp1 = std::chrono::steady_clock::now();
        for (int i = 0; i < 1000000; i++)
        {
            AddTask<CTestTask1>(&CTestTask1::Task1, &CTestTask1::Clear1, i);
        }
        auto tp2 = std::chrono::steady_clock::now();
        std::cout << "AddTask1:" << std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count() << std::endl;
    }

    void Task1(int nIndex)
    {
        // std::cout << "Task1:" << nIndex << std::endl;
    }
    void Clear1(int nIndex)
    {
        // std::cout << "Clear1:" << nIndex << std::endl;
    }
};


class CTestTask2 : public Arrow::Pattern::TaskOneThread2
{
public :
    CTestTask2(){}
    virtual ~CTestTask2(){}

public:

    void Push(int index)
    {
        AddTask<CTestTask2>(&CTestTask2::Task1, &CTestTask2::Clear1, index);
    }

    void Test()
    {
        auto tp1 = std::chrono::steady_clock::now();
        for (int i = 0; i < 1000000; i++)
        {
            AddTask<CTestTask2>(&CTestTask2::Task1, &CTestTask2::Clear1, i);
        }
        auto tp2 = std::chrono::steady_clock::now();
        std::cout << "AddTask2:" << std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count() << std::endl;
    }

    void Task1(int nIndex)
    {
        // std::cout << "Task1:" << nIndex << std::endl;
    }
    void Clear1(int nIndex)
    {
        // std::cout << "Clear1:" << nIndex << std::endl;
    }
};


template<typename T>
void ThreadInput(T* pTest, int nCount, int nIndex)
{
    auto tp1 = std::chrono::steady_clock::now();
    for (int i = 0; i < 1000000; i++)
    {
        pTest->Push(i * nCount + nIndex);
    }
    auto tp2 = std::chrono::steady_clock::now();
    std::cout << "Input:" << nIndex << ":" << std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count() << std::endl;

}

void TestTask()
{
    int nInputThread = 10;
    CTestTask1 task1;
    task1.Activate();

    // task1.Test();
    CTestTask2 task2;
    task2.Activate();
    // task2.Test();
    
    std::vector<std::thread> vThread;
    for(int i = 0; i < nInputThread; ++i)
    {
        // vThread.push_back(std::thread(std::bind(ThreadInput<CTestTask2>, &task2, nInputThread, i)));
        vThread.push_back(std::thread(std::bind(ThreadInput<CTestTask1>, &task1, nInputThread, i)));
    }

    // auto pFun = std::bind(ThreadInput<CTestTask2>, &task2, std::placeholders::_1);
    // pFun(1);

    std::this_thread::sleep_for(std::chrono::seconds(5));

    for(auto& it : vThread)
    {
        it.join();
    }

}