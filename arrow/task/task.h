#pragma once
#include <list>
#include <functional>
#include <tuple>
#include <thread>
#include <iostream>
#include <atomic>
#include <condition_variable>
#include <chrono>

namespace Arrow
{

namespace Pattern
{

// 此类类似框架类，接口函数都比较特殊，所以不提供访问接口，只有子类能对其进行访问 [zhuyb 2022-07-05 08:57:47]
template <int ThreadCount>
class TaskThread
{
protected:
    typedef std::function<void()> RunFunAddress;            // 任务执行函数定义 [zhuyb 2022-07-05 08:59:14]
    typedef std::function<void()> ClearCacheFunAddress;     // 内存回收函数定义 当线程结束，并且缓冲区还存在数据的时候就由此函数进行内存回收 [zhuyb 2022-07-05 08:59:32]
    typedef std::tuple<RunFunAddress, ClearCacheFunAddress> TaskFun;    // 单个任务的执行信息 [zhuyb 2022-07-05 09:00:12]
    typedef std::list<TaskFun> List_Task;   // 缓冲区，存储所有的任务 [zhuyb 2022-07-05 09:00:43]
protected:
    TaskThread():m_bIsRun(false)
    {
        
    }
    virtual ~TaskThread()
    {
        Stop();
    }

    bool Activate()
    {
        if (m_bIsRun == true)
        {
            return false;
        }
        
        m_bIsRun = true;
        m_Thread = std::thread(std::bind(&TaskOneThread::RunThread, this));
        WaitBeforeThreadRun();
        return true;
    }

    bool Stop()
    {
        if (!m_Thread.joinable())
        {
            return true;
        }
        printf("TaskOneThread::Stop Begin\n");

        m_bIsRun = false;
        m_Thread.join();
        List_Task::iterator it = m_listTask.begin();
        for (; it != m_listTask.end(); ++it)
        {
            std::get<1>(*it)();
        }
        m_listTask.clear();
        printf("TaskOneThread::Stop End\n");
        return true;
    }


    /**
     * @description: 等待线程执行完BeforeThreadRun
     * @param {uint32_t} un32TimeOutMs 超时时间 单位：毫秒 0 表示一等待
     * @return {*} true：BeforeThreadRun顺利执行完毕 false：超时返回
     */
    virtual bool WaitBeforeThreadRun(uint32_t un32TimeOutMs = 0)
    {

        std::unique_lock<std::mutex> lck(m_mutexBeforeThreadRun);

        if (un32TimeOutMs == 0)
        {
            m_cvBeforeThreadRun.wait(lck);
            return true;
        }
        
        return m_cvBeforeThreadRun.wait_until(
                   lck, std::chrono::system_clock::now() + std::chrono::milliseconds(un32TimeOutMs)) ==
               std::cv_status::no_timeout;
    }

    // 在线程进入循环前执行相关初始化操作 [zhuyb 2022-07-05 09:01:03]
    virtual void BeforeThreadRun(){}

    // 线程退出循环后执行响应的回收操作 [zhuyb 2022-07-05 09:01:25]
    virtual void AfterThreadStop(){}

    //   [zhuyb 2022-07-05 09:02:01]
    /**
     * @description: 
     * @param {TRunFun} pRunFun 执行函数地址
     * @param {TClearCacheFun} pClearCacheFun 数据回收函数。 当线程退出，缓存区还存在数据的时候，就会调用此函数
     * @param {_Args...} args 调用参数，由于是异步调用，使用者需要保证参数的什么周期。
     * @return {*} 添加任务 使用自定义回收函数
     */
    template<typename T, typename TRunFun, typename TClearCacheFun, typename ..._Args>
    bool AddTask1(TRunFun pRunFun, TClearCacheFun pClearCacheFun, _Args... args)
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

    // 添加任务 使用默认回收函数 回收函数只在线程退出后，缓存区还存在数据的时候才会调用 [zhuyb 2022-07-05 09:02:23]
    template<typename T, typename TRunFun, typename ..._Args>
    bool AddTask2(TRunFun pRunFun, _Args... args)
    {
        if(m_bIsRun == false)
        {
            return false;
        }

        TaskFun taskFun(std::bind(pRunFun, (T*)this, args...),
                        std::bind(&TaskOneThread::DefaultClearCache<_Args...>, (TaskOneThread*)this, args...));

        m_mutexListTask.lock();
        m_listTask.push_back(taskFun);
        m_mutexListTask.unlock();
        
        return true;
    }

private:
    // 默认回收函数 [zhuyb 2022-07-05 09:14:41]
    template<typename ..._Args>
    void DefaultClearCache(_Args... args)
    {
        printf("Call Default Data Delete\n");
    }

    void RunThread()
    {
        printf("Task Thread Start\n");
        BeforeThreadRun();

        m_cvBeforeThreadRun.notify_all();
        m_mutexBeforeThreadRun.unlock();


        List_Task tmpListTask;
        List_Task::iterator it;
        while (m_bIsRun)
        {
            if (m_listTask.size() == 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));//睡眠1000毫秒（1秒）           
                continue;
            }
            m_mutexListTask.lock();
            tmpListTask = m_listTask;
            m_listTask.clear();
            m_mutexListTask.unlock();

            for (it = tmpListTask.begin(); it != tmpListTask.end(); ++it)
            {
                std::get<0>(*it)();
                // 
            }
            tmpListTask.clear();
        }

        // 执行回收操作，回收未处理的数据 [zhuyb 2022-07-05 09:14:09]
        m_mutexListTask.lock();
        for (it = m_listTask.begin(); it != m_listTask.end(); ++it)
        {
            std::get<1>(*it)();
        }
        m_mutexListTask.unlock();

        AfterThreadStop();
        printf("Task Thread End\n");
    }

private:
    //  [zhuyb 2022-09-14 10:10:18]
    std::atomic<bool> m_bIsRun;
    std::thread m_Thread;
    std::mutex m_mutexListTask;
    std::mutex m_mutexBeforeThreadRun;
    std::condition_variable m_cvBeforeThreadRun;
    List_Task m_listTask;
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
