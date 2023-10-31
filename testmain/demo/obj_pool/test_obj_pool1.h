#pragma once
#include <iostream>
#include "arrow/arrow.h"

class CTestObj
{
public:
	CTestObj()
    {
        std::cout << "CTestObj()" << this << std::endl;
    }
	virtual ~CTestObj()
    {
        std::cout << "~CTestObj()" << this << std::endl;
    }

    void Release()
    {
        std::cout << "Release()" << this << std::endl;
    }
};
typedef Arrow::Pattern::Singleton<Arrow::Other::TObjSimplePool<CTestObj>> AppTestObjPool1;
typedef Arrow::Pattern::Singleton<Arrow::Other::TObjSimplePool2<CTestObj>> AppTestObjPool2;

static void TestObjPool1()
{
    CTestObj* pTmp = AppTestObjPool1::Instance()->Alloc();
    AppTestObjPool1::Instance()->Free(pTmp);

    pTmp = AppTestObjPool2::Instance()->Alloc();
    AppTestObjPool2::Instance()->Free(pTmp);
}