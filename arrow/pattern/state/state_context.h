/*
 * @FilePath: /arrowlib/arrow/pattern/state/state_context.h
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
 * @MsgEnum     消息
 * @return {*}
 */
template <typename StateData, typename StateEnum, typename MsgEnum>
class StateBase
{
protected:
    StateBase() {}

public:
    virtual ~StateBase() {}

    /**
     * @description: 初始化 创建状态的时候调用一次
     * @param {StateData&} data
     * @return {*}
     */
    virtual void Init(const StateData& data) = 0;

    /**
     * @description: 当从其他状态迁移到当前状态调用
     * @param {StateData*} pData
     * @return {*}
     */
    virtual void StateStart(StateData* pData) = 0;

    /**
     * @description: 处理消息
     * @param {StateData*} pData
     * @param {MsgEnum} msg
     * @return {*}
     */
    virtual StateEnum HandleMsg(StateData* pData, MsgEnum msg) = 0;

    /**
     * @description: 处理定时消息
     * @param {StateData*} pData
     * @return {*}
     */    
    virtual StateEnum Timer(StateData* pData) = 0;


    /**
     * @description: 当前状态
     * @return {*}
     */
    virtual StateEnum State() = 0;
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
template <typename StateData, typename StateEnum, typename MsgEnum, typename StateFactory, int32_t TimerInterval>
class StateContext{};

/**
 * @description: 使用偏特化写法，主要是为了约束工厂为StaticFactory
 * @return {*}
 */
template <typename StateData, typename StateEnum, typename MsgEnum, int32_t TimerInterval, typename ...Args>
class StateContext<StateData, StateEnum, MsgEnum, StaticFactory<Args...>,  TimerInterval>
{
protected:
    using _StateBase = StateBase<StateData, StateEnum, MsgEnum>;
    using StateFactory = StaticFactory<Args...>;
    using Local = StateContext<StateData, StateEnum, MsgEnum, StateFactory, TimerInterval>;
    using MapStateEnumToStatePtr = std::map<StateEnum, _StateBase*>;

public:
    StateContext() {}

    virtual ~StateContext()
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
        m_mapStateEnumToStatePtr[initState] = StateFactory::Create(initState);
        m_mapStateEnumToStatePtr[initState]->Init(data);

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
    virtual StateEnum HandleMsg(MsgEnum msg)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);

        auto* pStateBase = GetStateBasePtr(m_CurrentState);
        auto stateTmp =pStateBase->HandleMsg(&m_Data, msg);
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
        auto* pStateBase = GetStateBasePtr(newState);
        pStateBase->StateStart(&m_Data);
        m_PreviousState = m_CurrentState;
        m_CurrentState = newState;
    }

    /**
     * @description: 获取状态实现
     * @param {StateEnum&} state
     * @return {*}
     */
    _StateBase* GetStateBasePtr(const StateEnum& state)
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

    std::mutex m_Mutex;
    MapStateEnumToStatePtr m_mapStateEnumToStatePtr;
    StateEnum m_CurrentState;
    StateEnum m_PreviousState;
};
}
}