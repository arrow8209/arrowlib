#include "test_task.h"
#include <string>

CTestTask::CTestTask()
{
}

CTestTask::~CTestTask()
{
}

void CTestTask::Start()
{
    Arrow::Pattern::TaskOneThread::Activate("CMaxApplication");
}

void CTestTask::End()
{
    Arrow::Pattern::TaskOneThread::Stop();
}

void CTestTask::Stop(const char* szTaskID)
{
    std::cout << "Stop:" <<  szTaskID << std::endl;
    std::string strTaskID(szTaskID);
    AddTaskClearCacheNull<CTestTask>(&CTestTask::Task_Stop, strTaskID);
}

void CTestTask::Task_Stop(std::string strTaskID)
{
    std::cout << "TaskID:" << strTaskID << std::endl;
}
