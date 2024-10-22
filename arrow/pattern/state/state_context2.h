/*
 * @FilePath: arrow/pattern/state/state_context.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2023-11-21 17:53:41
 * @Description: 同步状态模式 需配合静态工厂使用，需要使用通过Timer接口触发定时器
 */
#pragma once
#include <stdint.h>
#include <map>
#include <thread>
#include <functional>
#include <chrono>
#include <mutex>
#include "../factory/static_factory.h"

namespace Arrow
{

namespace Pattern
{

namespace detail
{
template<typename _StateData, typename _StateEnum, typename _MsgEnum, typename ...Args>
class IContext2
{
public:
    using Data = _StateData;
    using StateEnum = _StateEnum;
    using Msg = _MsgEnum;

protected:
    IContext2() {}

public:
    virtual ~IContext2() {}

public:
    virtual StateEnum HandleMsg(Msg msg, Args... args) = 0;
    virtual void ChangeState(const StateEnum& newState) = 0;
};
}
/**
 * @description: 状态基类。接口函数线程安全，无需考虑多线程
 * @StateData   状态机全局数据类型
 * @StateEnum   状态枚举
 * @MsgEnum     消息枚举类似
 * @return {*}
 */
template <typename _StateData, typename _StateEnum, typename _MsgEnum, typename... Args>
class StateBase2
{
public:
    using Data = _StateData;
    using StateEnum = _StateEnum;
    using Msg = _MsgEnum;
    using ContextType = detail::IContext2<Data, StateEnum, Msg, Args...>;

    friend ContextType;

protected:
    StateBase2() {}

public:
    virtual ~StateBase2() {}

    void Context(ContextType* pContext, Data* pData)
    {
        m_pContext = pContext;
        m_pData = pData;
    }

    void ChangeState(const StateEnum& newState)
    {
        m_pContext->ChangeState(newState);
    }

    /**
     * @description: 初始化 创建状态的时候调用一次
     * @return {*}
     */
    virtual void Init() = 0;

    /**
     * @description: 当从其他状态迁移到当前状态调用
     * @return {*}
     */
    virtual void Start() = 0;

    /**
     * @description: 处理消息
     * @param {MsgEnum} msg
     * @return {*}
     */
    virtual void HandleMsg(Msg msg, Args... args) = 0;

    /**
     * @description: 处理定时消息
     * @return {*}
     */    
    virtual void Timer() = 0;

    /**
     * @description: 获取当前状态
     * @return {*}
     */
    virtual StateEnum State() = 0;

protected:
    ContextType* m_pContext = nullptr;
    Data* m_pData = nullptr;
};

/**
 * @description:
 * @StateData       状态机数据
 * @StateEnum       状态枚举
 * @StateFactory    状态机工厂
 * @MsgEnum         消息枚举值
 * @TimerInterval   定时器时间间隔；单位毫秒；0无需定时器
 * @return {*}
 */
template <typename StateData, typename StateEnum, typename MsgEnum, typename StateFactory, int32_t TimerInterval = 0, typename ...Args>
class StateContext2
{
};
/**
 * @description: 使用偏特化写法，主要是为了约束工厂为StaticFactory
 * @return {*}
 */
template <typename StateData, typename StateEnum, typename MsgEnum, int32_t TimerInterval, typename... Args1, typename... Args>
class StateContext2<StateData, StateEnum, MsgEnum, StaticFactory<Args1...>, TimerInterval, Args...>
    : public detail::IContext2<StateData, StateEnum, MsgEnum, Args...>
{
protected:
    using StateFactory = StaticFactory<Args1...>;
    using ContextType = detail::IContext2<StateData, StateEnum, MsgEnum, Args...>;
    using Local = StateContext2<StateData, StateEnum, MsgEnum, StateFactory, TimerInterval, Args...>;
    using Base = detail::IContext2<StateData, StateEnum, MsgEnum, Args...>;
    using StateBaseImpl = StateBase2<StateData, StateEnum, MsgEnum, Args...>;
    using MapStateEnumToStatePtr = std::map<StateEnum, StateBaseImpl*>;

public:
    StateContext2() {}

    virtual ~StateContext2()
    {
        if(m_ThreadTimer.joinable())
        {
            m_bThreadRun = false;
            m_ThreadTimer.join();
        }

        for(auto it : m_mapStateEnumToStatePtr)
        {
            delete it.second;
        }
        m_mapStateEnumToStatePtr.clear();
    }

    /**
     * @description: 初始化
     * @param {StateData&} data 状态机共享数据
     * @param {StateEnum} initState 初始状态
     * @param {StateOutImpl} pStateOut 状态返回接收器
     * @return {*} true：成功 false：失败（重复初始化）
     */
    virtual bool Init(const StateData& data, StateEnum initState)
    {
        if(m_bThreadRun == true)
        {
            return false;
        }

        m_Data = data;
        m_CurrentState = initState;
        m_PreviousState = initState;
        GetStateBasePtr(initState);

        m_bThreadRun = true;
        if (TimerInterval > 0)
        {
            m_ThreadTimer = std::thread(std::bind(&StateContext2::ThreadTimer, this));
        }
        return true;
    }
    
    /**
     * @description: 消息处理
     * @param {MsgEnum} msg 
     * @return {*}
     */
    virtual StateEnum HandleMsg(MsgEnum msg, Args... args)
    {
        std::lock_guard<std::recursive_mutex> guard(m_Mutex);

        auto* pStateBase = GetStateBasePtr(m_CurrentState);
        pStateBase->HandleMsg(msg, args...);

        return State();

    }

    /**
     * @description: 设置当前状态
     * @param {StateEnum&} newState
     * @return {*}
     */
    virtual void ChangeState(const StateEnum& newState)
    {
        std::lock_guard<std::recursive_mutex> guard(m_Mutex);
        if (newState == m_CurrentState)
        {
            return;
        }
        auto* pStateBase = GetStateBasePtr(newState);
        pStateBase->Start();

        m_PreviousState = m_CurrentState;
        m_CurrentState = newState;
    }

    /**
     * @description: 定时器
     * @return {*}
     */
    virtual StateEnum Timer()
    {
        std::lock_guard<std::recursive_mutex> guard(m_Mutex);
        
        auto* pStateBase = GetStateBasePtr(m_CurrentState);
        pStateBase->Timer();
        return State();
    }

    /**
     * @description: 获取当前状态
     * @return {*}
     */
    StateEnum State()
    {
        return m_CurrentState;
    }

    /**
     * @description: 获取上一次状态
     * @return {*}
     */
    StateEnum PreviousState()
    {
        return m_PreviousState;
    }

    const StateData& Data() const
    {
        return m_Data;
    }

protected:
    /**
     * @description: 获取状态实现
     * @param {StateEnum&} state
     * @return {*}
     */
    StateBaseImpl* GetStateBasePtr(const StateEnum& state)
    {
        auto it = m_mapStateEnumToStatePtr.find(state);
        if (it == m_mapStateEnumToStatePtr.end())
        {
            m_mapStateEnumToStatePtr[state] = StateFactory::Create(state);
            m_mapStateEnumToStatePtr[state]->Context(dynamic_cast<Base*>(this), &m_Data);
            m_mapStateEnumToStatePtr[state]->Init();
            
            return m_mapStateEnumToStatePtr[state];
        }
        return it->second;
    }
private:
    /**
     * @description: 定时器线程
     * @return {*}
     */
    void ThreadTimer()
    {
        auto tpStart = std::chrono::steady_clock::now();
        auto tpNow = tpStart;
        while(m_bThreadRun)
        {
            tpNow = std::chrono::steady_clock::now();
            auto tpSpace = std::chrono::duration_cast<std::chrono::milliseconds>(tpNow - tpStart).count();
            if(tpSpace >= TimerInterval)
            {
                tpStart = tpNow;
                Timer();
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

protected:
    bool m_bThreadRun = false;
    std::thread m_ThreadTimer;
    StateData m_Data;

    std::recursive_mutex m_Mutex;
    MapStateEnumToStatePtr m_mapStateEnumToStatePtr;
    StateEnum m_CurrentState;
    StateEnum m_PreviousState;
};
}
}