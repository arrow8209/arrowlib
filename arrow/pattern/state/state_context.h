/*
 * @FilePath: arrow/pattern/state/state_context.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2023-11-21 17:53:41
 * @Description: 状态模式 需配合静态工厂使用
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

/**
 * @description: 状态基类。接口函数线程安全，无需考虑多线程
 * @StateData   状态机全局数据类型
 * @StateEnum   状态枚举
 * @MsgEnum     消息枚举类似
 * @return {*}
 */
template <typename _StateData, typename _StateEnum, typename _MsgEnum, typename ...Args>
class StateBase
{
public:
    using Data = _StateData;
    using StateEnum = _StateEnum;
    using Msg = _MsgEnum;

protected:
    StateBase() {}

public:
    virtual ~StateBase() {}

    /**
     * @description: 初始化 创建状态的时候调用一次
     * @param {Data&} data
     * @return {*}
     */
    virtual void Init(const Data& data) = 0;

    /**
     * @description: 当从其他状态迁移到当前状态调用
     * @param {Data*} pData
     * @return {*}
     */
    virtual void Start(Data* pData) = 0;

    /**
     * @description: 处理消息
     * @param {Data*} pData
     * @param {MsgEnum} msg
     * @return {*}
     */
    virtual StateEnum HandleMsg(Data* pData, Msg msg, Args... args) = 0;

    /**
     * @description: 处理定时消息
     * @param {Data*} pData
     * @return {*}
     */    
    virtual StateEnum Timer(Data* pData) = 0;


    /**
     * @description: 当前状态
     * @return {*}
     */
    virtual StateEnum State() = 0;
};

template<typename _StateEnum>
class StateOutBase
{
public:
    using StateEnum = _StateEnum;

protected:
    StateOutBase() {}

public:
    virtual ~StateOutBase() {}

    virtual void StateChange(StateEnum oldState, StateEnum newState) = 0;
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
class StateContext
{};
/**
 * @description: 使用偏特化写法，主要是为了约束工厂为StaticFactory
 * @return {*}
 */
template <typename StateData, typename StateEnum, typename MsgEnum, int32_t TimerInterval,typename ...Args1, typename ...Args>
class StateContext<StateData, StateEnum, MsgEnum, StaticFactory<Args1...>, TimerInterval, Args...>
{
protected:
    using StateBaseImpl = StateBase<StateData, StateEnum, MsgEnum, Args...>;
    using StateOutImpl = StateOutBase<StateEnum>;
    using StateFactory = StaticFactory<Args1...>;
    using Local = StateContext<StateData, StateEnum, MsgEnum, StateFactory, TimerInterval,  Args...>;
    using MapStateEnumToStatePtr = std::map<StateEnum, StateBaseImpl*>;

public:
    StateContext() {}

    virtual ~StateContext()
    {
        m_pStateOut = nullptr;
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
    virtual bool Init(const StateData& data, StateEnum initState, StateOutImpl* pStateOut = nullptr)
    {
        if(m_bThreadRun == true)
        {
            return false;
        }

        m_Data = data;
        m_CurrentState = initState;
        m_PreviousState = initState;
        m_mapStateEnumToStatePtr[initState] = StateFactory::Create(initState);
        m_mapStateEnumToStatePtr[initState]->Init(data);
        m_pStateOut = pStateOut;

        m_bThreadRun = true;
        if (TimerInterval > 0)
        {
            m_ThreadTimer = std::thread(std::bind(&StateContext::ThreadTimer, this));
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
        std::lock_guard<std::mutex> guard(m_Mutex);

        auto* pStateBase = GetStateBasePtr(m_CurrentState);
        auto stateTmp =pStateBase->HandleMsg(&m_Data, msg, args...);
        State(stateTmp);

        return State();

    }

    /**
     * @description: 定时器
     * @return {*}
     */
    virtual StateEnum Timer()
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        
        auto* pStateBase = GetStateBasePtr(m_CurrentState);
        auto stateTmp = pStateBase->Timer(&m_Data);
        State(stateTmp);

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
     * @description: 设置当前状态
     * @param {StateEnum&} newState
     * @return {*}
     */
    void State(const StateEnum& newState)
    {
        if (newState == m_CurrentState)
        {
            return;
        }
        if(m_pStateOut != nullptr)
        {
            m_pStateOut->StateChange(m_CurrentState, newState);
        }
        auto* pStateBase = GetStateBasePtr(newState);
        pStateBase->Start(&m_Data);
        m_PreviousState = m_CurrentState;
        m_CurrentState = newState;
    }

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
            m_mapStateEnumToStatePtr[state]->Init(m_Data);
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

    StateOutImpl* m_pStateOut = nullptr;

    std::mutex m_Mutex;
    MapStateEnumToStatePtr m_mapStateEnumToStatePtr;
    StateEnum m_CurrentState;
    StateEnum m_PreviousState;
};
}
}