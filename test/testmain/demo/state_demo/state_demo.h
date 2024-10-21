#pragma once
#include <iostream>
#include <chrono>
#include "arrow/arrow.h"


enum class LightState
{
    Off,
    On
};
using LightStateStr = Arrow::EnumListToStr<LightState, LightState::Off, LightState::On>;

enum class LightMsg
{
    Down
};

struct LightData
{
    int nDownCountToOn = 2; // 连续按2次按钮灯开 [zhuyb 2023-11-21 15:11:06]
    int nDownCountToOff= 3; // 连续按3次按钮灯关 [zhuyb 2023-11-21 15:11:25]
    int nLightOnTimerLenght = 3; // 灯开3秒自动关
};

using LightStateBase = Arrow::Pattern::StateBase<LightData, LightState, LightMsg>;
using LightStateOutBase = Arrow::Pattern::StateOutBase<LightState>;

class LightOn : public LightStateBase
{
public:
    LightOn() {}
    virtual ~LightOn() {}

    virtual void Init(const LightData& data)
    {
        std::cout << "Init" << std::endl;
    }

    virtual void Start(LightData* pData)
    {
        std::cout << "On::Start" << std::endl;
        m_tpStart = std::chrono::steady_clock::now();
        m_nDownCount = 0;
    }

    virtual LightState HandleMsg(LightData* pData, LightMsg msg)
    {

        m_nDownCount++;
        if(m_nDownCount >= pData->nDownCountToOn)
        {
            std::cout << "On->Off" << std::endl;
            return LightState::Off;
        }
        std::cout << "On->On" << std::endl;
        return LightState::On;
    }

    virtual LightState Timer(LightData* pData)
    {
        auto tpNow = std::chrono::steady_clock::now();
        auto ts = std::chrono::duration_cast<std::chrono::seconds>(tpNow - m_tpStart).count();
        if(ts >= pData-> nLightOnTimerLenght)
        {
            std::cout << "Timer:On->Off:" << ts << std::endl;
            return LightState::Off;
        }
        std::cout << "Timer:On->On" << std::endl;
        return LightState::On;
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

    virtual void Init(const LightData& data)
    {
        std::cout << "Init" << std::endl;
    }

    virtual void Start(LightData* pData)
    {
        std::cout << "Off::Start" << std::endl;
        m_nDownCount = 0;
    }

    virtual LightState HandleMsg(LightData* pData, LightMsg msg)
    {

        m_nDownCount++;
        if(m_nDownCount >= pData->nDownCountToOff)
        {
            std::cout << "Off->On" << std::endl;
            return LightState::On;
        }
        std::cout << "Off->Off" << std::endl;
        return LightState::Off;
    }

    virtual LightState Timer(LightData* pData)
    {
        return LightState::Off;
    }

    virtual LightState State()
    {
        return LightState::Off;
    }

private:
    int m_nDownCount = 0;
};

class LigthOut : public LightStateOutBase
{
public:
    LigthOut() {}
    virtual ~LigthOut() {}

    virtual void StateChange(LightState oldState, LightState newState)
    {
        std::cout << LightStateStr::ItemStr(oldState) << ":" << LightStateStr::ItemStr(newState) << std::endl;
    }
};

using LightStateFactory = Arrow::Pattern::StaticFactory<LightState,
                                                        LightStateBase,
                                                        Arrow::Pattern::StaticFactoryAssist<LightState, LightState::On, LightOn>,
                                                        Arrow::Pattern::StaticFactoryAssist<LightState, LightState::Off, LightOff>>;

using LightStateContext = Arrow::Pattern::StateContext<LightData, LightState, LightMsg, LightStateFactory, 1000>;

void StateDemo()
{
    LightStateContext lightStateContext;
    LightData lightData;
    LigthOut lightOut;
    lightStateContext.Init(lightData, LightState::Off, dynamic_cast<LightStateOutBase*>(&lightOut));
    while(true)
    {
        std::string strTmp;
        std::cin >> strTmp;
        if(strTmp == "q")
            break;
        lightStateContext.HandleMsg(LightMsg::Down);
    }
}