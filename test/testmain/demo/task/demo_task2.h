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
private:
    uint32_t m_u32RunCount = 0;
};

static void DemoTask2()
{

    CDemoTask2 testTask;
    testTask.Start();

    std::string strTmp = "afsdfasdfasdf";
    testTask.AddTimer();

    while (true)
    // for(int i = 0; i < 1000000; i++)
    {   
        if(testTask.TaskCount() < 1000)
        {
            testTask.PushTask("1");
            testTask.PushTask("2");
            testTask.PushTask("3");
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::microseconds(1));
            // std::cout << "Task count: " << testTask.TaskCount() << std::endl;
        }
        // std::cout << testTask.TaskCount() << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cin >> strTmp;
    testTask.Stop();
}