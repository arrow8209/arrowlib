#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include "Observer.h"

namespace Arrow
{

namespace Pattern
{

template <typename _Ret, typename ..._Args>
class ISubject
{
public:
    typedef IObserver<_Ret, _Args...> Observer;
    typedef std::vector<Observer*> Vec_LPObserver;
public:
    ISubject(){}
    virtual ~ISubject(){}

    virtual bool Register(Observer* pObserver)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        typename Vec_LPObserver::iterator it = std::find(m_vecLPObserver.begin(), m_vecLPObserver.end(), pObserver);

        if (it != m_vecLPObserver.end())
        {
            return false;
        }

        m_vecLPObserver.push_back(pObserver);
        return true;
    }

    virtual bool UnRegister(Observer* pObserver)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        typename Vec_LPObserver::iterator it = std::find(m_vecLPObserver.begin(), m_vecLPObserver.end(), pObserver);

        if (it == m_vecLPObserver.end())
        {
            return false;
        }

        m_vecLPObserver.erase(it);
        return true;
    }

    virtual _Ret Notify(_Args... args)
    {
        m_Mutex.lock();
        Vec_LPObserver tmpVecLPObserver = m_vecLPObserver;
        m_Mutex.unlock();
        typename Vec_LPObserver::iterator it = tmpVecLPObserver.begin();
        for (; it != tmpVecLPObserver.end(); ++it)
        {
            (*it)->Response(args...);
        }
    }

public:
    std::mutex m_Mutex;
    Vec_LPObserver m_vecLPObserver;

};

//Demo
//class UserClass: public ISubject<parem1, parem2, pamre3 ... parmen>
// {
//      parem1 UserFun()
//        {
//            return Notify(parem2, pamre3 ... parmen)
//        }
// }
}
}