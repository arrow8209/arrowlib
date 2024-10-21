#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "arrow/arrow.h"

class CTestObj
{
public:
	CTestObj()
    {
        std::cout << "CTestObj():" << m_str << std::endl;
    }
	virtual ~CTestObj()
    {
        std::cout << "~CTestObj():" << m_str << std::endl;
        m_vec.clear();
    }

    void Release()
    {
        m_str = "abc";
        std::cout << "Release():" << m_str << std::endl;
    }

public:
    std::string m_str{"123456"};
    std::vector<int> m_vec;
    int m_n = 0;
};

typedef Arrow::Other::TObjSimplePool<CTestObj> CTestObjPool;
typedef Arrow::Pattern::Singleton<Arrow::Other::TObjSimplePool<CTestObj>> AppTestObjPool1;

static void TestObjPool1()
{
    CTestObj* pTmp = AppTestObjPool1::Instance()->Alloc();
    pTmp->m_vec.push_back(1);
    pTmp->m_vec.push_back(2);
    pTmp->m_vec.push_back(3);
    pTmp->m_n = 100;
    AppTestObjPool1::Instance()->Free(pTmp);

    pTmp = AppTestObjPool1::Instance()->Alloc();
    pTmp->m_vec.push_back(4);
    pTmp->m_vec.push_back(5);
    pTmp->m_vec.push_back(6);
    AppTestObjPool1::Instance()->Free(pTmp);

    pTmp = AppTestObjPool1::Instance()->Alloc();
    pTmp->m_vec.push_back(7);
    pTmp->m_vec.push_back(8);
    pTmp->m_vec.push_back(9);
    AppTestObjPool1::Instance()->Free(pTmp);
}

static void TestObjPool2()
{
    CTestObjPool* pPool = new CTestObjPool();
    CTestObj* pTmp = pPool->Alloc();
    pTmp->m_vec.push_back(1);
    pTmp->m_vec.push_back(2);
    pTmp->m_vec.push_back(3);
    pTmp->m_n = 100;
    pPool->Free(pTmp);

    pTmp = pPool->Alloc();
    pTmp->m_vec.push_back(4);
    pTmp->m_vec.push_back(5);
    pTmp->m_vec.push_back(6);
    pPool->Free(pTmp);

    pTmp = pPool->Alloc();
    pTmp->m_vec.push_back(7);
    pTmp->m_vec.push_back(8);
    pTmp->m_vec.push_back(9);
    pPool->Free(pTmp);

    delete pPool;
}