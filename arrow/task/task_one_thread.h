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
#include <vector>
#include "../other/delete_args.h"
#include "arrow/other/std_assist.h"

namespace Arrow
{

namespace Pattern
{

// 此类近似于框架类，接口函数都比较特殊，所以不提供访问接口，只有子类能对其进行访问 [zhuyb 2022-07-05 08:57:47]
class TaskOneThread
{
protected:
    typedef std::function<void()> RunFunAddress;            // 任务执行函数定义 [zhuyb 2022-07-05 08:59:14]
    typedef std::function<void()> ClearCacheFunAddress;     // 内存回收函数定义 当线程结束，并且缓冲区还存在数据的时候就由此函数进行内存回收 [zhuyb 2022-07-05 08:59:32]
    typedef std::tuple<RunFunAddress, ClearCacheFunAddress> TaskFun;    // 单个任务的执行信息 [zhuyb 2022-07-05 09:00:12]
    typedef std::list<TaskFun> List_Task;   // 缓冲区，存储所有的任务 [zhuyb 2022-07-05 09:00:43]

    typedef std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> time_point_milliseconds;
    // 定时器数据（定时器ID，触发间隔，1次触发，多次触发, 下次触发绝对时间） [zhuyb 2023-03-08 15:32:28]
    typedef std::tuple<
        int,                       // 定时器ID [zhuyb 2023-03-09 09:12:04]
        std::chrono::milliseconds, // 触发间隔 [zhuyb 2023-03-09 09:12:10]
        bool,                      // 触发方式 true:触发一次 false:触发多少 [zhuyb 2023-03-09 09:12:17]
        time_point_milliseconds, // 下次触发的绝对时间 [zhuyb 2023-03-09 09:13:03]
        RunFunAddress,        // 触发函数地址 [zhuyb 2023-03-09 09:13:19]
        ClearCacheFunAddress> // 参数回收函数地址 [zhuyb 2023-03-09 09:13:34]
        TimerData;

    constexpr static int TimerID = 0;
    constexpr static int TimerInterval = 1;
    constexpr static int TimerOnce = 2;
    constexpr static int TimerNextTP = 3;
    constexpr static int TimerRunAddr = 4;
    constexpr static int TimerClearAddr = 5;

    // 记录所有定时器 [zhuyb 2023-03-09 09:14:47]
    typedef std::vector<TimerData> VecTimer;
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
        // if (szThreadName != nullptr)
        // {
        //     m_strThreadName = szThreadName;
        //     pthread_setname_np(m_Thread.native_handle(), m_strThreadName.c_str());
        // }
        // else
        // {
        //     pthread_setname_np(m_Thread.native_handle(), m_strThreadName.c_str());
        // }

        // 等待线程完全启动 [zhuyb 2023-03-09 09:20:04]
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

        ClearTask();
        ClearTimer();

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
     * @param {_Args...} args 调用参数，由于是异步调用，使用者需要保证参数的生命周期。
     * @return {*} 添加任务 使用自定义回收函数
     */
    template<typename T, typename TRunFun, typename TClearCacheFun, typename ..._Args>
    bool AddTask(TRunFun pRunFun, TClearCacheFun pClearCacheFun, _Args... args)
    {
        if (m_bIsRun == false)
        {
            return false;
        }
        
        TaskFun taskFun(std::bind(pRunFun, (T*)this, std::forward<_Args>(args)...),
                        std::bind(pClearCacheFun, (T*)this, std::forward<_Args>(args)...));

        m_mutexListTask.lock();
        m_listTask.push_back(taskFun);
        m_mutexListTask.unlock();

        return true;
    }

    // 添加任务 使用空回收函数 回收函数只在线程退出后，缓存区还存在数据的时候才会调用 [zhuyb 2022-07-05 09:02:23]
    template<typename T, typename TRunFun, typename ..._Args>
    bool AddTaskClearCacheNull(TRunFun pRunFun, _Args... args)
    {
        return AddTask<T>(pRunFun, &TaskOneThread::ClearCacheNull<_Args...>, std::forward<_Args>(args)...);

        // if (m_bIsRun == false)
        // {
        //     return false;
        // }

        // TaskFun taskFun(std::bind(pRunFun, (T*)this, std::forward<_Args>(args)...),
        //                 std::bind(&TaskOneThread::ClearCacheNull, (TaskOneThread*)this));

        // m_mutexListTask.lock();
        // m_listTask.push_back(taskFun);
        // m_mutexListTask.unlock();

        // return true;
    }

    // 添加任务 使用delete 回收函数 线程退出后，缓存区还存在数据的时候会对缓存数据 调用 delete args [zhuyb 2022-07-05 09:02:23]
    template<typename T, typename TRunFun, typename ..._Args>
    bool AddTaskClearCacheDelete(TRunFun pRunFun, _Args... args)
    {
        return AddTask<T>(pRunFun, &TaskOneThread::ClearCacheDelete<_Args...>, std::forward<_Args>(args)...);

    }


    //   [zhuyb 2022-07-05 09:02:01]
    /**
     * @description: 
     * @param {bool} bOnce 触发次数 true 触发一次 false 触发多次
     * @param {uint32_t} u32Interval 触发间隔，单位毫秒（触发精度为10毫秒，非高精度定时器）
     * @param {TRunFun} pRunFun 执行函数地址
     * @param {TClearCacheFun} pClearCacheFun 数据回收函数。 当线程退出，缓存区还存在数据的时候，就会调用此函数
     * @param {_Args...} args 调用参数，由于是异步调用，使用者需要保证参数的生命周期。
     * @return {*} 添加任务 使用自定义回收函数
     */
    template<typename T, typename TRunFun, typename TClearCacheFun, typename ..._Args>
    bool AddTimer(bool bOnce, uint32_t u32Interval, TRunFun pRunFun, TClearCacheFun pClearCacheFun, _Args... args)
    {
        std::chrono::milliseconds millInterval(u32Interval);
        time_point_milliseconds tNext =
            std::chrono::time_point_cast<time_point_milliseconds::duration>(std::chrono::system_clock::now());
        TimerData timerData(++m_u32TimeID,
                            millInterval,
                            bOnce,
                            tNext,
                            std::bind(pRunFun, (T*)this, std::forward<_Args>(args)...),
                            std::bind(pClearCacheFun, (T*)this, std::forward<_Args>(args)...));

        m_mutextVecTimer.lock();
        m_vecTimer.push_back(timerData);
        m_mutextVecTimer.unlock();

        return true;
    }

    // 添加任务 使用空回收函数 回收函数只在线程退出后，缓存区还存在数据的时候才会调用 [zhuyb 2022-07-05 09:02:23]
    template<typename T, typename TRunFun, typename ..._Args>
    bool AddTimerClearCacheNull(bool bOnce, uint32_t u32Interval, TRunFun pRunFun, _Args... args)
    {
        return AddTimer<T>(
            bOnce, u32Interval, pRunFun, &TaskOneThread::ClearCacheNull<_Args...>, std::forward<_Args>(args)...);

        // if (m_bIsRun == false)
        // {
        //     return false;
        // }
        // std::chrono::milliseconds millInterval(u32Interval);
        // time_point_milliseconds tNext =
        //     std::chrono::time_point_cast<time_point_milliseconds::duration>(std::chrono::system_clock::now());
        // TimerData timerData(++m_u32TimeID,
        //                     millInterval,
        //                     bOnce,
        //                     tNext,
        //                     std::bind(pRunFun, (T*)this, std::forward<_Args>(args)...),
        //                     std::bind(&TaskOneThread::ClearCacheNull, (TaskOneThread*)this));

        // m_mutextVecTimer.lock();
        // m_vecTimer.push_back(timerData);
        // m_mutextVecTimer.unlock();

        // return true;
    }

    // 添加任务 使用delete 回收函数 线程退出后，缓存区还存在数据的时候会对缓存数据 调用 delete args [zhuyb 2022-07-05 09:02:23]
    template<typename T, typename TRunFun, typename ..._Args>
    bool AddTimerClearCacheDelete(bool bOnce, uint32_t u32Interval, TRunFun pRunFun, _Args... args)
    {
        return AddTimer<T>(bOnce, u32Interval, pRunFun, &TaskOneThread::ClearCacheDelete<_Args...>, std::forward<_Args>(args)...);

    }

    // 在线程进入循环前执行相关初始化操作 [zhuyb 2022-07-05 09:01:03]
    virtual void BeforeThreadRun(){}

    // 线程退出循环后执行响应的回收操作 [zhuyb 2022-07-05 09:01:25]
    virtual void AfterThreadStop(){}

private:
    void WaitBeforeThreadRun()
    {
        m_FutureBeforeThreadRun.get();
    }

    // 默认回收函数 [zhuyb 2022-07-05 09:14:41]
    template <typename... Args>
    void ClearCacheNull(Args&... args)
    {
        printf("%s call null clear catch data\n", m_strThreadName.c_str());
    }

    template<typename ...Args>
    void ClearCacheDelete(Args&... args)
    {
        Arrow::Other::DeleteArgs(args...);
        printf("%s call delete clear catch data\n", m_strThreadName.c_str());
    }

    void RunThread()
    {
        printf("%s Task Thread Start\n", m_strThreadName.c_str());

        Arrow::Other::SetCurrentThreadName(m_strThreadName);
        BeforeThreadRun();
        // 标记线程已启动并已经完成初始化 [zhuyb 2023-03-09 09:18:08]
        m_PromiseBeforThreadRun.set_value(true);

        List_Task tmpListTask;

        while (m_bIsRun)
        {
            if (m_listTask.empty() == true)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                RunThreadTimer();
                continue;
            }

            RunThreadTask();
            RunThreadTimer();
        }

        printf("%s Task Thread End Start\n", m_strThreadName.c_str());

        ClearTask();
        ClearTimer();
        AfterThreadStop();
        printf("%s Task Thread End\n", m_strThreadName.c_str());
    }
    
    /**
     * @description: 执行所有的线程任务
     * @return {*}
     */
    void RunThreadTask()
    {
        if(m_listTask.size() == 0)
            return;

        // 每次只执行一个任务，是为了快速响应线程退出的信号 [zhuyb 2022-09-14 17:05:09]
        m_mutexListTask.lock();
        TaskFun fun = m_listTask.front();
        m_listTask.pop_front();
        m_mutexListTask.unlock();
        
        std::get<0>(fun)();
    }

    /**
     * @description: 线程结束清除所有任务
     * @return {*}
     */
    void ClearTask()
    {
        List_Task::iterator it;
        // 执行回收操作，回收未处理的数据 [zhuyb 2022-07-05 09:14:09]
        m_mutexListTask.lock();
        for (it = m_listTask.begin(); it != m_listTask.end(); ++it)
        {
            std::get<1>(*it)();
        }
        m_listTask.clear();
        m_mutexListTask.unlock();
    }

    /**
     * @description: 运行所有定时器
     * @return {*}
     */
    void RunThreadTimer()
    {
        if(m_vecTimer.size() == 0)
            return;

        m_mutextVecTimer.lock();
        VecTimer vecTmpTimer(std::move(m_vecTimer));
        m_mutextVecTimer.unlock();

        time_point_milliseconds tNow =
            std::chrono::time_point_cast<time_point_milliseconds::duration>(std::chrono::system_clock::now());
        std::vector<VecTimer::iterator> vecEraseTimer;
        for(auto it = vecTmpTimer.begin(); it != vecTmpTimer.end(); it++)
        {
            if(tNow < std::get<TimerNextTP>(*it))
            {
                continue;
            }
            // 获取执行函数地址 [zhuyb 2023-03-09 09:50:27]
            std::get<TimerRunAddr>(*it)();

            // 如果执行一次,记录timerid。后续方便删除 [zhuyb 2023-03-09 09:51:50]
            if( std::get<TimerOnce>(*it) == true)
            {
                vecEraseTimer.push_back(it);
            }
            else
            {
                // 修改下次触发时间 [zhuyb 2023-03-09 09:55:42]
                auto& tpNext = std::get<TimerNextTP>(*it);
                tpNext += std::get<TimerInterval>(*it);
            }
        }

        // 删除失效定时器 [zhuyb 2023-03-09 09:56:36]
        for (auto& val :vecEraseTimer)
        {
            vecTmpTimer.erase(val);
        }

        m_mutextVecTimer.lock();
        m_vecTimer = std::move(vecTmpTimer);
        m_mutextVecTimer.unlock();
    }

    /**
     * @description: 清除定时器队列数据
     * @return {*}
     */
    void ClearTimer()
    {
        m_mutextVecTimer.lock();
        for (auto& val : m_vecTimer)
        {
            std::get<TimerClearAddr>(val)();
        }
        
        m_mutextVecTimer.unlock();
    }
private:
    std::atomic<bool> m_bIsRun; // 线程运行标志 [zhuyb 2022-09-14 10:10:40]
    std::thread m_Thread; // 线程对象 [zhuyb 2022-09-14 10:10:55]
    std::string m_strThreadName = "ArrowTask"; // 线程名称 [zhuyb 2022-10-20 11:08:36]

    std::mutex m_mutexListTask; // 任务列表锁 [zhuyb 2022-09-14 10:11:10]
    List_Task m_listTask;       // 任务列表数据 [zhuyb 2023-03-09 09:44:33]

    std::mutex m_mutextVecTimer; // 定时器数据锁 [zhuyb 2023-03-09 09:15:11]
    VecTimer m_vecTimer;         // 定时器数据 [zhuyb 2023-03-09 09:44:51]
    uint32_t m_u32TimeID;        // 定时器自增ID [zhuyb 2023-03-09 09:45:01]

    // 使用 futrue和promise来模拟线程信号量同步 [zhuyb 2023-03-09 09:15:35]
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
