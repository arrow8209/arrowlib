#include "demo_task2.h"
#include <iostream>
#include <string>

CDemoTask2::CDemoTask2()
{
}

CDemoTask2::~CDemoTask2()
{
    Stop();
}

void CDemoTask2::Start()
{
    TaskThread::Activate("CDemoTask2");
}

void CDemoTask2::Stop()
{
    TaskThread::Stop();
}

void CDemoTask2::AddTimer()
{
    TaskThread::AddTimerClearCacheNull(false, 500, &CDemoTask2::Timer);
}

void CDemoTask2::PushTask(const char* szTaskID)
{
    std::string strTaskID(szTaskID);
    // TaskThread::AddTaskClearCacheNull(&CDemoTask2::RunTask, strTaskID);
    TaskThread::AddTask(&CDemoTask2::RunTask, &CDemoTask2::ClearTask, strTaskID);
}

void CDemoTask2::RunTask(std::string strTaskID)
{
    std::cout << "RunTaskID:" << strTaskID << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void CDemoTask2::ClearTask(std::string strTaskID)
{
    std::cout << "ClearTaskID:" << strTaskID << std::endl;
}

void CDemoTask2::Timer()
{
    std::cout << "Timer:" << std::endl;
}
