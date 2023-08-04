#pragma once
#include "arrow/task/task_one_thread.h"

class CTestTask : public Arrow::Pattern::TaskOneThread
{
public:
    CTestTask();
    virtual ~CTestTask();

    void Start();

    void End();

    void Stop(const char* szTaskID);

protected:

    void Task_Stop(std::string strTaskID);
};


static void RunTestTask()
{

    typedef void (CTestTask::*FunStopPtr)(const char*);


    CTestTask testTask;
    testTask.Start();

    std::string strTmp = "afsdfasdfasdf";
    // FunStopPtr pFunPtr = &CTestTask::Stop;
    // pFunPtr(&testTask, strTmp.c_str());
    //(testTask.*pFunPtr)(strTmp.c_str());

    testTask.Stop(strTmp.c_str());

    std::cin>> strTmp;
    testTask.End();

}