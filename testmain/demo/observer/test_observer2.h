#pragma once
#include <functional>
#include <iostream>
#include "arrow/arrow.h"

using ISubject2Demo = Arrow::Pattern::Subject2<void, int>;

class CSubject2 : public ISubject2Demo
{
public:
	CSubject2()
	{

	}

	virtual ~CSubject2()
	{
    }

    void Push(int i)
    {
        ISubject2Demo::Notify(i);
    }
};

class CObserver2
{

public:
    CObserver2()
    {
    }

    virtual ~CObserver2()
    {
    }

    void Response1(int i)
    {
        std::cout << "CObserver2::Response1::" << i << std::endl;
    }

    void Response2(int i)
    {
        std::cout << "CObserver2::Response2::" << i << std::endl;
    }

    void Response3(int i, int n)
    {
        std::cout << "CObserver2::Response3::" << i << ":" << n << std::endl;
    }
};

void TestObserver2()
{
    CSubject2 subject;
    CObserver2 observer;

    subject.Register(&observer, &CObserver2::Response1);
    subject.Register(&observer, &CObserver2::Response1);
    subject.Register(&observer, &CObserver2::Response2);
    subject.Register(&observer, &CObserver2::Response2);

    ISubject2Demo::ObserverFunc pFun1 = std::bind(&CObserver2::Response3, &observer, std::placeholders::_1, 5);
    subject.Register(pFun1);
    ISubject2Demo::ObserverFunc pFun2 = std::bind(&CObserver2::Response3, &observer, std::placeholders::_1, 6);
    subject.Register(pFun2);
    
    subject.Push(1);
}