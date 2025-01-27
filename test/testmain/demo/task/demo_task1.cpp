#include "demo_task1.h"
#include <iostream>
#include <string>

CDemoTask1::CDemoTask1()
{
}

CDemoTask1::~CDemoTask1()
{
    Stop();
}

void CDemoTask1::Start()
{
    TaskThread::Activate("CDemoTask1");
}

void CDemoTask1::Stop()
{
    TaskThread::Stop();
}

void CDemoTask1::AddTimer()
{
    TaskThread::AddTimerClearCacheNull<CDemoTask1>(false, 500, &CDemoTask1::Timer);
}

void CDemoTask1::PushTask(const char* szTaskID)
{
    std::string strTaskID(szTaskID);
    // TaskThread::AddTaskClearCacheNull(&CDemoTask1::RunTask, strTaskID);
    TaskThread::AddTask<CDemoTask1>(&CDemoTask1::RunTask, &CDemoTask1::ClearTask, strTaskID);
}

void CDemoTask1::RunTask(std::string strTaskID)
{
    std::cout << "RunTaskID:" << strTaskID << std::endl;
    // std::this_thread::sleep_for(std::chrono::seconds(1));
}

void CDemoTask1::ClearTask(std::string strTaskID)
{
    std::cout << "ClearTaskID:" << strTaskID << std::endl;
}

void CDemoTask1::Timer()
{
    std::cout << "Timer:" << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() << std::endl;
}
