#pragma once
#include <functional>
#include <iostream>
#include <tuple>
#include "arrow/arrow.h"

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