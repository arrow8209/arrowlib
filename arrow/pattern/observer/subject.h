/*
 * @FilePath: /arrow/pattern/observer/subject.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-12-07 10:08:16
 * @Description: 简单观察者模式
 */
#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>

namespace Arrow
{

namespace Pattern
{

template <typename _Ret, typename ..._Args>
class Observer
{
public:
    Observer() {}

    virtual ~Observer() {}

public:
    virtual _Ret Response(_Args... _args) = 0;
};

template <typename _Ret, typename ..._Args>
class Subject
{
public:
    typedef Observer<_Ret, _Args...> IObserver;

protected:
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

    virtual bool UnRegister()
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        m_vecLPObserver.clear();
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

protected:
    std::mutex m_Mutex;
    Vec_PtrObserver m_vecLPObserver;

};

/* Demo
using ISubjectDemo = Arrow::Pattern::Subject<void, int>;
using IObserverDemo = ISubjectDemo::IObserver;

class CSubject : public ISubjectDemo
{
public:
	CSubject()
	{

	}

	virtual ~CSubject()
	{
    }

    void Push(int i)
    {
        ISubjectDemo::Notify(i);
    }
};

class CObserver : public IObserverDemo
{

public:
    CObserver()
    {
    }

    virtual ~CObserver()
    {
    }

    void Response(int i)
    {
        std::cout << "CObserver::Reponse1::" << i << std::endl;
    }

};

void TestObserver1()
{
    CSubject subject;
    CObserver observer;

    subject.Register(dynamic_cast<IObserverDemo*>(&observer));

    subject.Push(1);
}
*/
}
}