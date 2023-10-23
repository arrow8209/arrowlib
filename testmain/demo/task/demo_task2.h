#pragma once
#include "arrow/task/task_one_thread.h"
#include "arrow/task/task_one_thread2.h"

class CDemoTask2 : public Arrow::Pattern::TaskOneThread2<CDemoTask2>
{

    using TaskThread = Arrow::Pattern::TaskOneThread2<CDemoTask2>;
public:
    CDemoTask2();
    virtual ~CDemoTask2();

    void Start();

    void Stop();

    void AddTimer();

    void PushTask(const char* szTaskID);
private:
    void RunTask(std::string strTaskID);

    void ClearTask(std::string strTaskID);

    void Timer();
};

static void DemoTask2()
{

    CDemoTask2 testTask;
    testTask.Start();

    std::string strTmp = "afsdfasdfasdf";
    testTask.AddTimer();
    testTask.PushTask("1");
    testTask.PushTask("2");
    testTask.PushTask("3");

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cin >> strTmp;
    testTask.Stop();
}