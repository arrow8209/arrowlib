#pragma once
#include <chrono>
#include "arrow/other/static_cast.h"
#include "arrow/other/delete_args.h"
#include "arrow/task/task_one_thread.h"

// template<typename ...Args>
// struct ADeleteObj;

// template<typename T, typename ...Args>
// struct ADeleteObj<T,  Args...>
// {
//     static void Del(T&, Args&... args)
//     {   
//         ADeleteObj<Args...>::Del(args...);
//     }
// };

// template<typename T, typename ...Args>
// struct ADeleteObj<T*,  Args...>
// {
//     static void Del(T*& pObj, Args&... args)
//     {   
//         delete pObj;
//         ADeleteObj<Args...>::Del(args...);
//     }
// };

// template<typename T>
// struct ADeleteObj<T>
// {
//     static void Del(T&)
//     {   

//     }
// };

// template<typename T>
// struct ADeleteObj<T*>
// {
//     static void Del(T*& pObj)
//     {   
//         delete pObj;
//     }
// };

// 以下的写法存在编译错误 [zhuyb 2023-01-14 11:42:30]
// template <typename T, typename... Args>
// void ADeleteObj(T* pObj, Args... args)
// {
//     // ADeleteObj(args...);
// }

// template <typename T, typename... Args>
// void ADelteObj(T pObj, Args... args)
// {
//     // ADeleteObj(args...);
// }

// template <typename T>
// void ADeleteObj(T* pObj)
// {
//     // delete pObj;
// }

// template <typename T>
// void ADeleteObj(T pObj)
// {
// }

class CTestDelCatch
{
public:
    CTestDelCatch(int n1){ m=n1;}

    ~CTestDelCatch()
    {
        printf("~CTestDelCatch %d\n", m);
    }
    void Delete()
    {
        printf("Call Delete %d\n", m);
    }
    int m;
};

class CTestTask: public Arrow::Pattern::TaskOneThread
{
public:

    void TstAddTask()
    {
        // AddTask<CTestTask>(&CTestTask::Print, &CTestTask::PrintDestory, (int)1);
        // AddTaskClearCacheNull<CTestTask>(&CTestTask::Print2);

        CTestDelCatch* pT1 = new CTestDelCatch(1);
        CTestDelCatch* pT2 = new CTestDelCatch(2);
        CTestDelCatch* pT3 = new CTestDelCatch(3);
        CTestDelCatch* pT4 = new CTestDelCatch(4);
        
        int nTmp = 53243;
        AddTask<CTestTask>(&CTestTask::Print, &CTestTask::PrintDestory, (int)1);
        AddTaskClearCacheDelete<CTestTask>(&CTestTask::Print, nTmp);
        AddTaskClearCacheDelete<CTestTask>(&CTestTask::TaskDel1, pT1);
        AddTaskClearCacheDelete<CTestTask>(&CTestTask::TaskDel2, pT2, pT3);
        AddTaskClearCacheDelete<CTestTask>(&CTestTask::TaskDel3, nTmp, pT4);
        // std::string* pStr = new std::string();
        // Arrow::Other::DeleteArgs(nTmp, pStr);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        Stop();
    }

    void TaskDel1(CTestDelCatch* p)
    {
        std::cout << "TaskDel1" << std::endl;
        delete p;
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }

    void TaskDel2(CTestDelCatch* p1,CTestDelCatch* p2)
    {
        std::cout << "TaskDel2" << std::endl;
    }

    void TaskDel3(int nTmp, CTestDelCatch* p1)
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