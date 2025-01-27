#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>

namespace Arrow
{

namespace Pattern
{
template<typename ...Args>
class Handler
{
private:
    using Local = Handler<Args...>;
protected:
    Handler() {}

public:
    virtual ~Handler() {}

    /// @brief 
    /// @param ...args 
    /// @return 
    virtual void Handle(Args... args) = 0;

    void Push(Args... args)
    {
        if(m_pNext == nullptr)
            return;
        m_pNext->Handle(args...);
    }

    Local* Next(Local* pNext)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        Local* pRet = m_pNext;
        m_pNext = pNext;
        return pRet;
    }

    Local* Next()
    {
        m_Mutex.lock();
        Local* pRet = m_pNext;
        m_Mutex.unlock();
        return pRet;
    }

private:
    std::mutex m_Mutex;
    Local* m_pNext = nullptr;
};

}
}

/*
#pragma once
#include "arrow/arrow.h"
#include <random>

using IResponsibilityBase = Arrow::Pattern::Handler<int>;
using ISubjectBase = Arrow::Pattern::Subject<void, int>;
using IObserverBase = ISubjectBase::IObserver;

enum class ResponsibilityEnum
{
    _emStart = 0,
    _emResponsibility1,
    _emResponsibility2,
    _emResponsibility3,
    _emEnd
};

enum class ObserverEnum
{
    _emStart = 0,
    _emObserver1,
    _emObserver2,
    _emObserver3,
    _emEnd
};

class Responsibility1 : public IResponsibilityBase
{
public:
    virtual ~Responsibility1() 
    {
        std::cout << "Responsibility1::~Responsibility1()" << std::endl;
    }

    virtual void Handle(int data)
    {
        std::cout << "Responsibility1::Handle(" << data << ")" << std::endl;
        IResponsibilityBase::Push(data + 1);
    }
};

class Responsibility2 : public IResponsibilityBase
{
public:
    virtual ~Responsibility2() 
    {
        std::cout << "Responsibility2::~Responsibility2()" << std::endl;
    }

    virtual void Handle(int data)
    {
        std::cout << "Responsibility2::Handle(" << data << ")" << std::endl;
        IResponsibilityBase::Push(data + 1);
    }
};

class Responsibility3 : public IResponsibilityBase
{
public:
    virtual ~Responsibility3() 
    {
        std::cout << "Responsibility3::~Responsibility3()" << std::endl;
    }

    virtual void Handle(int data)
    {
        std::cout << "Responsibility3::Handle(" << data << ")" << std::endl;
        IResponsibilityBase::Push(data + 1);
    }
};

class ResponsibilityEnd : public IResponsibilityBase, public ISubjectBase
{
public:
    virtual ~ResponsibilityEnd()
    {
        std::cout << "ResponsibilityEnd::~ResponsibilityEnd()" << std::endl;
    }

    virtual void Handle(int data)
    {
        std::cout << "ResponsibilityEnd::Handle(" << data << ")" << std::endl;
        ISubjectBase::Notify(data + 1);
    }
};

class Observer1 : public IObserverBase
{
public:
    virtual ~Observer1()
    {
        std::cout << "Observer1::~Observer1()" << std::endl;
    }

    virtual void Response(int data)
    {
        std::cout << "Observer1::Update(" << data << ")" << std::endl;
    }
};

class Observer2 : public IObserverBase
{
public:
    virtual ~Observer2()
    {
        std::cout << "Observer2::~Observer2()" << std::endl;
    }

    virtual void Response(int data)
    {
        std::cout << "Observer2::Update(" << data << ")" << std::endl;
    }
};

class Observer3 : public IObserverBase
{
public:
    virtual ~Observer3()
    {
        std::cout << "Observer3::~Observer3()" << std::endl;
    }

    virtual void Response(int data)
    {
        std::cout << "Observer3::Update(" << data << ")" << std::endl;
    }
};

using ResponsibilityFactory = Arrow::Pattern::StaticFactory<ResponsibilityEnum,
                                                            IResponsibilityBase,
                                                            Arrow::Pattern::StaticFactoryAssist<ResponsibilityEnum, ResponsibilityEnum::_emResponsibility1, Responsibility1>,
                                                            Arrow::Pattern::StaticFactoryAssist<ResponsibilityEnum, ResponsibilityEnum::_emResponsibility2, Responsibility2>,
                                                            Arrow::Pattern::StaticFactoryAssist<ResponsibilityEnum, ResponsibilityEnum::_emResponsibility3, Responsibility3>>;

using ObserverFactory = Arrow::Pattern::StaticFactory<ObserverEnum,
                                                      IObserverBase,
                                                      Arrow::Pattern::StaticFactoryAssist<ObserverEnum, ObserverEnum::_emObserver1, Observer1>,
                                                      Arrow::Pattern::StaticFactoryAssist<ObserverEnum, ObserverEnum::_emObserver2, Observer2>,
                                                      Arrow::Pattern::StaticFactoryAssist<ObserverEnum, ObserverEnum::_emObserver3, Observer3>>;

void TestResponsibility1()
{
    std::random_device rd;  // 用于获取非确定性种子
    std::mt19937 gen(rd()); // 使用 Mersenne Twister 引擎，并用 rd() 播种

    // 创建一个均匀分布
    std::uniform_int_distribution<> distrib(1, 3); // 生成 1 到 100 之间的整数
    ResponsibilityEnum emType = static_cast<ResponsibilityEnum>(distrib(gen));

    std::vector<IResponsibilityBase*> vecResponsibilityList;
    std::vector<IObserverBase*> vecObserverList;

    IResponsibilityBase* pStart = ResponsibilityFactory::Create(emType);
    IResponsibilityBase* pEnd = pStart;
    vecResponsibilityList.push_back(pStart);
    for (size_t i = 0; i < 5; i++)
    {
        emType = static_cast<ResponsibilityEnum>(distrib(gen));
        IResponsibilityBase* pTmp = ResponsibilityFactory::Create(emType);
        vecResponsibilityList.push_back(pTmp);
        pEnd->Next(pTmp);
        pEnd = pTmp;
    }
    
    ResponsibilityEnd* pResponsibilityEnd = new ResponsibilityEnd();
    pEnd->Next(pResponsibilityEnd);
    vecResponsibilityList.push_back(pResponsibilityEnd);

    for (size_t i = 0; i < 5; i++)
    {
        IObserverBase* pObserver = ObserverFactory::Create(static_cast<ObserverEnum>(distrib(gen)));
        pResponsibilityEnd->Register(pObserver);
        vecObserverList.push_back(pObserver);
    }
    
    pStart->Handle(1);

    for (size_t i = 0; i < vecResponsibilityList.size(); i++)
    {
        delete vecResponsibilityList[i];
    }

    for (size_t i = 0; i < vecObserverList.size(); i++)
    {
        delete vecObserverList[i];
    }
}
*/