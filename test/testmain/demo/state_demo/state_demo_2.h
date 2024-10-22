#pragma once
#include <iostream>
#include <chrono>
#include "arrow/arrow.h"

namespace State_Demo_2_H
{
// 定义所有状态 [zhuyb 2024-10-22 11:05:34]
enum class LightState
{
    Off,
    On
};
using LightStateStr = ::Arrow::EnumListToStr<LightState, LightState::Off, LightState::On>;

// 定义所有消息 [zhuyb 2024-10-22 11:05:49]
enum class LightMsg
{
    Down
};

// 定义状态进共享数据 [zhuyb 2024-10-22 11:06:00]
struct LightData
{
    int nDownCountToOn = 2; // 连续按2次按钮灯开 [zhuyb 2023-11-21 15:11:06]
    int nDownCountToOff= 3; // 连续按3次按钮灯关 [zhuyb 2023-11-21 15:11:25]
    int nLightOnTimerLenght = 3; // 灯开3秒自动关
};

// 定义状态基类 [zhuyb 2024-10-22 11:06:15]
using LightStateBase = ::Arrow::Pattern::StateBase2<LightData, LightState, LightMsg>;

class LightOn : public LightStateBase
{
public:
    LightOn() {}
    virtual ~LightOn() {}

    virtual void Init()
    {
        std::cout << "Init" << std::endl;
    }

    virtual void Start()
    {
        std::cout << "On::Start" << std::endl;
        m_tpStart = std::chrono::steady_clock::now();
        m_nDownCount = 0;
    }

    virtual void HandleMsg(LightMsg msg)
    {
        m_nDownCount++;
        if (m_nDownCount >= m_pData->nDownCountToOn)
        {
            std::cout << "On->Off" << std::endl;
            ChangeState(LightState::Off);
            return;
        }
        std::cout << "On->On" << std::endl;
    }

    virtual void Timer()
    {
        auto tpNow = std::chrono::steady_clock::now();
        auto ts = std::chrono::duration_cast<std::chrono::seconds>(tpNow - m_tpStart).count();
        if(ts >= m_pData-> nLightOnTimerLenght)
        {
            std::cout << "Timer:On->Off:" << ts << std::endl;
            ChangeState(LightState::Off);
            return;
        }
        std::cout << "Timer:On->On" << std::endl;
    }

    virtual LightState State()
    {
        return LightState::On;
    }

private:
    std::chrono::steady_clock::time_point m_tpStart;
    int m_nDownCount = 0;
};

class LightOff : public LightStateBase
{
public:
    LightOff() {}
    virtual ~LightOff() {}

    virtual void Init()
    {
        std::cout << "Init" << std::endl;
    }

    virtual void Start()
    {
        std::cout << "Off::Start" << std::endl;
        m_nDownCount = 0;
    }

    virtual void HandleMsg(LightMsg msg)
    {

        m_nDownCount++;
        if(m_nDownCount >= m_pData->nDownCountToOff)
        {
            std::cout << "Off->On" << std::endl;
            ChangeState(LightState::On);
            return;
        }
        std::cout << "Off->Off" << std::endl;
    }

    virtual void Timer()
    {
        // return LightState::Off;
    }

    virtual LightState State()
    {
        return LightState::Off;
    }

private:
    int m_nDownCount = 0;
};

using LightStateFactory = ::Arrow::Pattern::StaticFactory<LightState,
                                                          LightStateBase,
                                                          ::Arrow::Pattern::StaticFactoryAssist<LightState, LightState::On, LightOn>,
                                                          ::Arrow::Pattern::StaticFactoryAssist<LightState, LightState::Off, LightOff>>;

using LightStateContext = ::Arrow::Pattern::StateContext2<LightData, LightState, LightMsg, LightStateFactory, 1000>;

void StateDemo()
{
    LightStateContext lightStateContext;
    LightData lightData;
    
    lightStateContext.Init(lightData, LightState::Off);
    while(true)
    {
        std::string strTmp;
        std::cin >> strTmp;
        if(strTmp == "q")
            break;
        lightStateContext.HandleMsg(LightMsg::Down);
    }
}

}