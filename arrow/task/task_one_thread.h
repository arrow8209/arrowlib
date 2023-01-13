#pragma once
#include <list>
#include <functional>
#include <tuple>
#include <thread>
#include <iostream>
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <string>
#include <pthread.h>
#include <future>
namespace Arrow
{

namespace Pattern
{

// 此类类似框架类，接口函数都比较特殊，所以不提供访问接口，只有子类能对其进行访问 [zhuyb 2022-07-05 08:57:47]
class TaskOneThread
{
protected:
    typedef std::function<void()> RunFunAddress;            // 任务执行函数定义 [zhuyb 2022-07-05 08:59:14]
    typedef std::function<void()> ClearCacheFunAddress;     // 内存回收函数定义 当线程结束，并且缓冲区还存在数据的时候就由此函数进行内存回收 [zhuyb 2022-07-05 08:59:32]
    typedef std::tuple<RunFunAddress, ClearCacheFunAddress> TaskFun;    // 单个任务的执行信息 [zhuyb 2022-07-05 09:00:12]
    typedef std::list<TaskFun> List_Task;   // 缓冲区，存储所有的任务 [zhuyb 2022-07-05 09:00:43]
protected:
    TaskOneThread():m_bIsRun(false)
    {
        
    }
    virtual ~TaskOneThread()
    {
        Stop();
    }
public:
    // 启动线程 [zhuyb 2022-09-14 17:09:34]
    bool Activate(const char* szThreadName = nullptr)
    {
        if (m_bIsRun == true)
        {
            return false;
        }
        
        m_bIsRun = true;
        m_FutureBeforeThreadRun = m_PromiseBeforThreadRun.get_future();
        m_Thread = std::thread(std::bind(&TaskOneThread::RunThread, this));
        if (szThreadName != nullptr)
        {
            m_strThreadName = szThreadName;
            pthread_setname_np(m_Thread.native_handle(), m_strThreadName.c_str());
        }
        else
        {
            pthread_setname_np(m_Thread.native_handle(), m_strThreadName.c_str());
        }

        WaitBeforeThreadRun();
        return true;
    }

    // 停止线程 [zhuyb 2022-09-14 17:07:46]
    bool Stop()
    {
        if (!m_Thread.joinable())
        {
            return true;
        }
        printf("%s TaskOneThread::Stop Begin\n", m_strThreadName.c_str());

        m_bIsRun = false;
        m_Thread.join();
        List_Task::iterator it = m_listTask.begin();
        for (; it != m_listTask.end(); ++it)
        {
            std::get<1>(*it)();
        }
        m_listTask.clear();
        printf("%s TaskOneThread::Stop End\n", m_strThreadName.c_str());
        return true;
    }

    // 设置线程停止标志 [zhuyb 2022-09-14 17:07:56]
    void StopFlag()
    {
        m_bIsRun = false;
    }

    uint32_t TaskCount()
    {
        return m_listTask.size();
    }

protected:

    //   [zhuyb 2022-07-05 09:02:01]
    /**
     * @description: 
     * @param {TRunFun} pRunFun 执行函数地址
     * @param {TClearCacheFun} pClearCacheFun 数据回收函数。 当线程退出，缓存区还存在数据的时候，就会调用此函数
     * @param {_Args...} args 调用参数，由于是异步调用，使用者需要保证参数的什么周期。
     * @return {*} 添加任务 使用自定义回收函数
     */
    template<typename T, typename TRunFun, typename TClearCacheFun, typename ..._Args>
    bool AddTask(TRunFun pRunFun, TClearCacheFun pClearCacheFun, _Args... args)
    {
        if (m_bIsRun == false)
        {
            return false;
        }
        
        TaskFun taskFun(std::bind(pRunFun, (T*)this, args...),
                        std::bind(pClearCacheFun, (T*)this, args...));

        m_mutexListTask.lock();
        m_listTask.push_back(taskFun);
        m_mutexListTask.unlock();

        return true;
    }

    // 添加任务 使用空回收函数 回收函数只在线程退出后，缓存区还存在数据的时候才会调用 [zhuyb 2022-07-05 09:02:23]
    template<typename T, typename TRunFun, typename ..._Args>
    bool AddTaskClearCacheNull(TRunFun pRunFun, _Args... args)
    {
        return AddTask<T>(pRunFun, &TaskOneThread::ClearCacheNull<_Args...>, args...);

        // if (m_bIsRun == false)
        // {
        //     return false;
        // }

        // TaskFun taskFun(std::bind(pRunFun, (T*)this, args...),
        //                 std::bind(&TaskOneThread::ClearCacheNull<_Args...>, (TaskOneThread*)this, args...));

        // m_mutexListTask.lock();
        // m_listTask.push_back(taskFun);
        // m_mutexListTask.unlock();

        // return true;
    }

    // 添加任务 使用delete 回收函数 线程退出后，缓存区还存在数据的时候会对缓存数据 调用 delete args [zhuyb 2022-07-05 09:02:23]
    template<typename T, typename TRunFun, typename ..._Args>
    bool AddTaskClearCacheDelete(TRunFun pRunFun, _Args... args)
    {
        return AddTask<T>(pRunFun, &TaskOneThread::ClearCacheDelete<_Args...>, args...);

    }

    // 在线程进入循环前执行相关初始化操作 [zhuyb 2022-07-05 09:01:03]
    virtual void BeforeThreadRun(){}

    // 线程退出循环后执行响应的回收操作 [zhuyb 2022-07-05 09:01:25]
    virtual void AfterThreadStop(){}

private:
    // 默认回收函数 [zhuyb 2022-07-05 09:14:41]
    template<typename ..._Args>
    void ClearCacheNull(_Args... args)
    {
        printf("%s call null clear catch data\n", m_strThreadName.c_str());
    }

    template<typename ...Args>
    void ClearCacheDelete(Args... args)
    {
        DeleteObj(args...);
        // std::initializer_list<int>{([&]{delete args; }(), 0)...};
        printf("%s call delete clear catch data\n", m_strThreadName.c_str());
    }

    template <typename T, typename ...Args>
    void DeleteObj(T* pObj, Args... args)
    {
        pObj->Delete();
        delete pObj;
        DeleteObj(args...);
    }

    template <typename T, typename ...Args>
    void DelteObj(T pObj, Args... args)
    {
        DeleteObj(args...);
    }

    template <typename T>
    void DeleteObj(T* pObj)
    {
        delete pObj;
    }

    template <typename T>
    void DeleteObj(T pObj)
    {
    }


    void RunThread()
    {
        printf("%s Task Thread Start\n", m_strThreadName.c_str());
        BeforeThreadRun();
        m_PromiseBeforThreadRun.set_value(true);

        List_Task tmpListTask;
        List_Task::iterator it;
        while (m_bIsRun)
        {
            if (m_listTask.empty() == true)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));           
                continue;
            }

            // 每次只执行一个任务，是为了快速响应线程退出的信号 [zhuyb 2022-09-14 17:05:09]
            m_mutexListTask.lock();
            TaskFun fun = m_listTask.front();
            m_listTask.pop_front();
            m_mutexListTask.unlock();
            
            std::get<0>(fun)();
        }

        printf("%s Task Thread End Start\n", m_strThreadName.c_str());
        // 执行回收操作，回收未处理的数据 [zhuyb 2022-07-05 09:14:09]
        m_mutexListTask.lock();
        for (it = m_listTask.begin(); it != m_listTask.end(); ++it)
        {
            std::get<1>(*it)();
        }
        m_listTask.clear();
        m_mutexListTask.unlock();

        AfterThreadStop();
        printf("%s Task Thread End\n", m_strThreadName.c_str());
    }
    
    void WaitBeforeThreadRun()
    {
        m_FutureBeforeThreadRun.get();
    }
private:
    // 线程运行标准 [zhuyb 2022-09-14 10:10:40]
    std::atomic<bool> m_bIsRun;
    // 线程对象 [zhuyb 2022-09-14 10:10:55]
    std::thread m_Thread;
    // 线程名称 [zhuyb 2022-10-20 11:08:36]
    std::string m_strThreadName = "ArrowTask";

    // 任务列表锁 [zhuyb 2022-09-14 10:11:10]
    std::mutex m_mutexListTask;
    List_Task m_listTask;

    std::future<bool> m_FutureBeforeThreadRun;
    std::promise<bool> m_PromiseBeforThreadRun;
};

// Demo
// class CTestTask: public TaskOneThread
// {
// public:

//     void TstAddTask()
//     {
//         AddTask1<CTestTask>(&CTestTask::Print, &CTestTask::PrintDestory, (int)1);
//         AddTask2<CTestTask>(&CTestTask::Print2);
//     }
//
//     void Print(int n1)
//     {
//         printf("2\n");
//     }
//
//     void PrintDestory(int n1)
//     {
//         printf("2\n");
//     }

//     void Print2()
//     {
//         printf("3\n");
//     }
// };

}
}
