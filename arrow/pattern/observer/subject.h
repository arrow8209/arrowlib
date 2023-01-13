#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include "observer.h"

namespace Arrow
{

namespace Pattern
{

template <typename _Ret, typename ..._Args>
class Subject
{
public:
    typedef Observer<_Ret, _Args...> IObserver;
    typedef std::vector<IObserver*> Vec_PtrObserver;
public:
    Subject(){}
    virtual ~Subject(){}

    virtual bool Register(IObserver* pObserver)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        typename Vec_PtrObserver::iterator it = std::find(m_vecLPObserver.begin(), m_vecLPObserver.end(), pObserver);

        if (it != m_vecLPObserver.end())
        {
            return false;
        }

        m_vecLPObserver.push_back(pObserver);
        return true;
    }

    virtual bool UnRegister(IObserver* pObserver)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        typename Vec_PtrObserver::iterator it = std::find(m_vecLPObserver.begin(), m_vecLPObserver.end(), pObserver);

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
        Vec_PtrObserver tmpVecLPObserver = m_vecLPObserver;
        m_Mutex.unlock();
        typename Vec_PtrObserver::iterator it = tmpVecLPObserver.begin();
        for (; it != tmpVecLPObserver.end(); ++it)
        {
            (*it)->Response(args...);
        }
    }

private:
    std::mutex m_Mutex;
    Vec_PtrObserver m_vecLPObserver;

};

//Demo
//class UserClass: public Subject<parem1, parem2, pamre3 ... parmen>
// {
//      parem1 UserFun()
//        {
//            return Notify(parem2, pamre3 ... parmen)
//        }
// }
}
}