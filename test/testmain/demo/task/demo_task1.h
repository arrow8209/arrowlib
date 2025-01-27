#pragma once
#include "arrow/task/task_one_thread.h"
#include "arrow/task/task_one_thread2.h"

class CDemoTask1 : public Arrow::Pattern::TaskOneThread
{

    using TaskThread = Arrow::Pattern::TaskOneThread;
public:
    CDemoTask1();
    virtual ~CDemoTask1();

    void Start();

    void Stop();

    void AddTimer();

    void PushTask(const char* szTaskID);
private:
    void RunTask(std::string strTaskID);

    void ClearTask(std::string strTaskID);

    void Timer();
};

static void DemoTask1()
{

    CDemoTask1 testTask;
    testTask.Start();

    std::string strTmp = "afsdfasdfasdf";
    testTask.AddTimer();

    while (true)
    {    
        testTask.PushTask("1");
        testTask.PushTask("2");
        testTask.PushTask("3");
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        std::cout << testTask.TaskCount() << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cin >> strTmp;
    testTask.Stop();
}