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
#include <assert.h>
#include "lock_free_queue.h"
#include "lock_queue.h"
#include "simplg_lock_queue.h"
#include "../other/increment.h"
#include "../other/delete_args.h"

namespace Arrow
{

namespace Pattern
{

// 此类近似于框架类，接口函数都比较特殊，所以不提供访问接口，只有子类能对其进行访问 TimerPrecision:定时器精度 单位毫秒 [zhuyb 2022-07-05 08:57:47]
template<typename T, uint32_t TimerPrecision>
class TaskOneThread_TimerPrecision2
{
protected:
    typedef T SubClassType;
    typedef TaskOneThread_TimerPrecision2<T, TimerPrecision> Local;

    typedef std::function<void(bool)> TaskFun; // 任务执行函数定义 bool true:执行逻辑函数 false 执行回收函数 [zhuyb 2022-07-05 08:59:14]
    typedef LockQueue<TaskFun*> MsqQueue;
    // typedef SimpleLockQueue<TaskFun*> MsqQueue;

    typedef Arrow::Other::TIncrement<Local, uint32_t> TimerIDIncrement;        
    typedef std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds> time_point_milliseconds;
    // typedef LockFreeQueue<int, std::chrono::milliseconds, bool, TaskFun*> TimerQueue;

    // 定时器数据（定时器ID，触发间隔，1次触发，多次触发, 下次触发绝对时间） [zhuyb 2023-03-08 15:32:28]
    using TimerData = std::tuple<
        int,                       // 定时器ID [zhuyb 2023-03-09 09:12:04]
        std::chrono::milliseconds, // 触发间隔 [zhuyb 2023-03-09 09:12:10]
        bool,                      // 触发方式 true:触发一次 false:触发多少 [zhuyb 2023-03-09 09:12:17]
        time_point_milliseconds,   // 下次触发的绝对时间 [zhuyb 2023-03-09 09:13:03]
        TaskFun*>;                 // 任务函数 [zhuyb 2023-03-09 09:13:34]

    constexpr static int TimerID = 0;
    constexpr static int TimerInterval = 1;
    constexpr static int TimerOnce = 2;
    constexpr static int TimerNextTP = 3;
    constexpr static int TimerRunAddr = 4;
    constexpr static int TimerClearAddr = 5;

    // 记录所有定时器 [zhuyb 2023-03-09 09:14:47]
    typedef std::vector<TimerData> VecTimer;
protected:
    TaskOneThread_TimerPrecision2():m_bIsRun(false)
    {
        
    }
    virtual ~TaskOneThread_TimerPrecision2()
    {
        Stop();
    }
public:
    // 
    /**
     * @description: 启动线程 [zhuyb 2022-09-14 17:09:34]
     * @param {char*} szThreadName 线程名称。注意Linux线程名称限制为16个字节包括结束符号\0
     * @return {*}
     */
    bool Activate(const char* szThreadName = nullptr)
    {
        if (m_bIsRun == true)
        {
            return false;
        }
        
        m_bIsRun = true;
        m_bIsBegin = false;
        m_Thread = std::thread(std::bind(&Local::RunThread, this));
        if (szThreadName != nullptr)
        {
            m_strThreadName = szThreadName;
        }
        m_QueueTask.SetName(m_strThreadName.c_str());
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
        printf("%s TaskOneThread::Stop %d\n", m_strThreadName.c_str(), __LINE__);

        m_bIsRun = false;
        m_Thread.join();
        printf("%s TaskOneThread::Stop %d\n", m_strThreadName.c_str(), __LINE__);

        ClearTask();
        printf("%s TaskOneThread::Stop %d\n", m_strThreadName.c_str(), __LINE__);

        ClearTimer();
        printf("%s TaskOneThread::Stop %d\n", m_strThreadName.c_str(), __LINE__);

        return true;
    }

    // 设置线程停止标志 [zhuyb 2022-09-14 17:07:56]
    void StopFlag()
    {
        m_bIsRun = false;
    }

    uint32_t TaskCount()
    {
        return m_QueueTask.Size();
        // return m_QueueTask.size();
    }

    bool IsRun()
    {
        return m_bIsRun.load();
    }

    void PrintDebugInfo()
    {
        printf("%s %10d %10d %10d %10d %10d\n",m_strThreadName.c_str(), m_n32Test1, m_n32Test2, m_n32Test3, m_n32Test4, m_n32Test5);
    }
protected:

    //   [zhuyb 2022-07-05 09:02:01]
    /**
     * @description: 
     * @param {TRunFun} pRunFun 执行函数地址
     * @param {TClearCacheFun} pClearCacheFun 数据回收函数。 当线程退出，缓存区还存在数据的时候，就会调用此函数
     * @param {Args...} args 调用参数，由于是异步调用，使用者需要保证参数的生命周期。
     * @return {*} 添加任务 使用自定义回收函数
     */
    template<typename TRunFun, typename TClearCacheFun, typename ...Args>
    bool AddTask(TRunFun pRunFun, TClearCacheFun pClearCacheFun, Args... args)
    {
        if (m_bIsRun == false)
        {
            return false;
        }
        // 不能使用完美转发，否则会丢数据。具体原因不清 [zhuyb 2023-07-11 19:09:44]
        TaskFun* pTaskFun = new TaskFun(std::bind(&Local::RunOneTask<TRunFun, TClearCacheFun, Args...>, (Local*)this, std::placeholders::_1, pRunFun, pClearCacheFun, args...));
        m_QueueTask.Push(pTaskFun);
        return true;
    }

    // 添加任务 使用空回收函数 回收函数只在线程退出后，缓存区还存在数据的时候才会调用 [zhuyb 2022-07-05 09:02:23]
    template<typename TRunFun, typename ...Args>
    bool AddTaskClearCacheNull(TRunFun pRunFun, Args... args)
    {
        return AddTask(pRunFun, &Local::ClearCacheNull<Args...>, std::forward<Args>(args)...);
    }

    // 添加任务 使用delete 回收函数 线程退出后，缓存区还存在数据的时候会对缓存数据 调用 delete args [zhuyb 2022-07-05 09:02:23]
    template<typename TRunFun, typename ...Args>
    bool AddTaskClearCacheDelete(TRunFun pRunFun, Args... args)
    {
        return AddTask(pRunFun, &Local::ClearCacheDelete<Args...>, std::forward<Args>(args)...);

    }

    //   [zhuyb 2022-07-05 09:02:01]
    /**
     * @description: 
     * @param {bool} bOnce 触发次数 true 触发一次 false 触发多次
     * @param {uint32_t} u32Interval 触发间隔，单位毫秒（触发精度为10毫秒，非高精度定时器）
     * @param {TRunFun} pRunFun 执行函数地址
     * @param {TClearCacheFun} pClearCacheFun 数据回收函数。 当线程退出，缓存区还存在数据的时候，就会调用此函数
     * @param {Args...} args 调用参数，由于是异步调用，使用者需要保证参数的生命周期。
     * @return {*} 添加任务 使用自定义回收函数
     */
    template<typename TRunFun, typename TClearCacheFun, typename ...Args>
    bool AddTimer(bool bOnce, uint32_t u32Interval, TRunFun pRunFun, TClearCacheFun pClearCacheFun, Args... args)
    {
        // 不能使用完美转发，会丢数据。具体原因不清 [zhuyb 2023-07-11 19:09:44]
        TaskFun* pTaskFun = new TaskFun(std::bind(&Local::RunOneTask<TRunFun, TClearCacheFun, Args...>, (Local*)this, std::placeholders::_1, pRunFun, pClearCacheFun, args...));
        std::chrono::milliseconds millInterval(u32Interval);
        time_point_milliseconds tNext =
            std::chrono::time_point_cast<time_point_milliseconds::duration>(std::chrono::steady_clock::now()) + millInterval;
        TimerData timerData(TimerIDIncrement::Get(),
                            millInterval,
                            bOnce,
                            tNext,
                            pTaskFun); 

        m_mutextVecTimer.lock();
        m_vecTimer.push_back(timerData);
        m_mutextVecTimer.unlock();

        return true;
    }

    // 添加任务 使用空回收函数 回收函数只在线程退出后，缓存区还存在数据的时候才会调用 [zhuyb 2022-07-05 09:02:23]
    template<typename TRunFun, typename ...Args>
    bool AddTimerClearCacheNull(bool bOnce, uint32_t u32Interval, TRunFun pRunFun, Args... args)
    {
        return AddTimer(
            bOnce, u32Interval, pRunFun, &Local::ClearCacheNull<Args...>, std::forward<Args>(args)...);
    }

    // 添加任务 使用delete 回收函数 线程退出后，缓存区还存在数据的时候会对缓存数据 调用 delete args [zhuyb 2022-07-05 09:02:23]
    template<typename TRunFun, typename ...Args>
    bool AddTimerClearCacheDelete(bool bOnce, uint32_t u32Interval, TRunFun pRunFun, Args... args)
    {
        return AddTimer(bOnce, u32Interval, pRunFun, &Local::ClearCacheDelete<Args...>, std::forward<Args>(args)...);
    }

    // 在线程进入循环前执行相关初始化操作 [zhuyb 2022-07-05 09:01:03]
    virtual void BeforeThreadRun(){}

    // 线程退出循环后执行响应的回收操作 [zhuyb 2022-07-05 09:01:25]
    virtual void AfterThreadStop(){}

private:
    void WaitBeforeThreadRun()
    {
        std::unique_lock<std::mutex> lock(m_mutexIsBegin);
        m_cvIsBegin.wait(lock, [this]()
                         { return m_bIsBegin; });
    }

    void RunThread()
    {
        printf("%s Task Thread RunThread:%d\n", m_strThreadName.c_str(), __LINE__);
        BeforeThreadRun();
        // 标记线程已启动并已经完成初始化 [zhuyb 2023-03-09 09:18:08]
        {
            std::unique_lock<std::mutex> lock(m_mutexIsBegin);
            m_bIsBegin = true;
            m_cvIsBegin.notify_all();
        }

        while (m_bIsRun)
        {
            if (m_QueueTask.Empty() == true)
            {
                std::this_thread::sleep_for(m_msTimerPrecision);
                RunThreadTimer();
                continue;
            }

            RunThreadTask();
            RunThreadTimer();
        }

        printf("%s Task Thread RunThread:%d\n", m_strThreadName.c_str(), __LINE__);

        ClearTask();
        printf("%s Task Thread RunThread:%d\n", m_strThreadName.c_str(), __LINE__);

        ClearTimer();
        printf("%s Task Thread RunThread:%d\n", m_strThreadName.c_str(), __LINE__);

        AfterThreadStop();
        printf("%s Task Thread RunThread:%d\n", m_strThreadName.c_str(), __LINE__);
    }

    /**
     * @description: 执行所有的线程任务
     * @return {*}
     */
    void RunThreadTask()
    {
        TaskFun* pFun = nullptr;
        if(m_QueueTask.Pop(pFun) != true)
            return;

        (*pFun)(true);
        delete pFun;
    }

     /**
     * @description: 线程结束清除所有任务
     * @return {*}
     */
    void ClearTask()
    {
        TaskFun* pFun = nullptr;
        while(m_QueueTask.Empty() == false)
        {
            if(m_QueueTask.Pop(pFun) == true)
            {
                (*pFun)(false);
                delete pFun;
            }
        }
    }

    template <typename TRunFun, typename TClearCacheFun, typename... Args>
    void RunOneTask(bool bIsRunFun, TRunFun pRunFun, TClearCacheFun pClearCacheFun, Args... args)
    {
        SubClassType* pSubClass = dynamic_cast<SubClassType*>(this);
        assert(pSubClass != nullptr);
        if(bIsRunFun == true)
        {
            (pSubClass->*pRunFun)(args...);
        }
        else
        {
            (pSubClass->*pClearCacheFun)(args...);
        }
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
            std::chrono::time_point_cast<time_point_milliseconds::duration>(std::chrono::steady_clock::now());
        std::vector<VecTimer::iterator> vecEraseTimer;
        for(auto it = vecTmpTimer.begin(); it != vecTmpTimer.end(); it++)
        {
            if(tNow < std::get<TimerNextTP>(*it))
            {
                continue;
            }
            // 获取执行函数地址 [zhuyb 2023-03-09 09:50:27]
            TaskFun* pFun = std::get<TimerRunAddr>(*it);
            (*pFun)(true);

            // 如果执行一次,记录timerid。后续方便删除 [zhuyb 2023-03-09 09:51:50]
            if( std::get<TimerOnce>(*it) == true)
            {
                vecEraseTimer.push_back(it);
            }
            else
            {
                // 修改下次触发时间 [zhuyb 2023-03-09 09:55:42]
                auto& tpNext = std::get<TimerNextTP>(*it);
                tpNext = tNow + std::get<TimerInterval>(*it);
            }
        }

        // 删除失效定时器 [zhuyb 2023-03-09 09:56:36]
        for (auto& val :vecEraseTimer)
        {
            TaskFun* pFun = std::get<TimerRunAddr>(*val);
            (*pFun)(false);
            delete pFun;
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
            TaskFun* pFun = std::get<TimerRunAddr>(val);
            (*pFun)(false);
            delete pFun;
        }
        m_vecTimer.clear();
        m_mutextVecTimer.unlock();
    }

        // 默认回收函数 [zhuyb 2022-07-05 09:14:41]
    template <typename... Args>
    void ClearCacheNull(Args... args)
    {
        printf("%s call null clear catch data\n", m_strThreadName.c_str());
    }

    template<typename ...Args>
    void ClearCacheDelete(Args... args)
    {
        Arrow::Other::DeleteArgs(args...);
        printf("%s call delete clear catch data\n", m_strThreadName.c_str());
    }
private:
    std::atomic<bool> m_bIsRun; // 线程运行标志 [zhuyb 2022-09-14 10:10:40]
    std::thread m_Thread; // 线程对象 [zhuyb 2022-09-14 10:10:55]
    std::string m_strThreadName = "ArrowTask"; // 线程名称 [zhuyb 2022-10-20 11:08:36]

    MsqQueue m_QueueTask;       // 任务列表数据 [zhuyb 2023-03-09 09:44:33]

    std::mutex m_mutextVecTimer;       // 定时器数据锁 [zhuyb 2023-03-09 09:15:11]
    VecTimer m_vecTimer;               // 定时器数据 [zhuyb 2023-03-09 09:44:51]

    // 线程启动前执行 状态 [zhuyb 2023-03-09 09:15:35]
    bool m_bIsBegin = false;
    std::mutex m_mutexIsBegin;
    std::condition_variable m_cvIsBegin;

    // 休眠时间 [zhuyb 2023-09-14 09:31:38]
    std::chrono::milliseconds m_msTimerPrecision{TimerPrecision};

    // test  [zhuyb 2024-08-21 10:42:57]
    int32_t m_n32Test1 = 0;
    int32_t m_n32Test2 = 0;
    int32_t m_n32Test3 = 0;
    int32_t m_n32Test4 = 0;
    int32_t m_n32Test5 = 0;
};

template<typename T>
using TaskOneThread2 = TaskOneThread_TimerPrecision2<T, 1>;

}
}
