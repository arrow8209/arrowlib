#pragma once
#include <chrono>
#include "arrow/task/task_one_thread.h"

class CTestDelCatch
{
public:
    CTestDelCatch() = default;

    ~CTestDelCatch()
    {
        printf("~CTestDelCatch\n");
    }
    void Delete()
    {
        printf("Call Delete\n");
    }
};

class CTestTask: public Arrow::Pattern::TaskOneThread
{
public:

    void TstAddTask()
    {
        AddTask<CTestTask>(&CTestTask::Print, &CTestTask::PrintDestory, (int)1);
        AddTaskClearCacheNull<CTestTask>(&CTestTask::Print2);

        CTestDelCatch* pT1 = new CTestDelCatch();
        CTestDelCatch* pT2 = new CTestDelCatch();
        CTestDelCatch* pT3 = new CTestDelCatch();
        CTestDelCatch* pT4 = new CTestDelCatch();
        
        AddTaskClearCacheDelete<CTestTask>(&CTestTask::TaskDel1, pT1);
        AddTaskClearCacheDelete<CTestTask>(&CTestTask::TaskDel2, pT2, pT3);
        AddTaskClearCacheDelete<CTestTask>(&CTestTask::TaskDel3, pT4, (int)585858);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        Stop();
    }

    void TaskDel1(CTestDelCatch* p)
    {
        delete p;
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }

    void TaskDel2(CTestDelCatch* p1,CTestDelCatch* p2)
    {
        std::cout << "TaskDel2" << std::endl;
    }

    void TaskDel3(CTestDelCatch* p1,int nTmp)
    {
        std::cout << "TaskDel3" << std::endl;
    }

    void Print(int n1)
    {
        printf("Print: 2\n");
    }

    void PrintDestory(int n1)
    {
        printf("PrintDestory\n");
    }

    void Print2()
    {
        printf("Print2:3\n");
    }


};