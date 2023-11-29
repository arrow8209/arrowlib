#pragma once
#include "arrow/arrow.h"
#include <iostream>

enum class ObjKeyEnum
{
    ObjKey0,
    ObjKey1,
    ObjKey2
};

class ObjBase
{
public:
    virtual ~ObjBase()
    {
        std::cout << "~ObjBase" << std::endl;
    }
    virtual void Show() = 0;
};

class Obj0 : public ObjBase
{
public:
    virtual ~Obj0()
    {
        std::cout << "~Obj0" << std::endl;
    }
    virtual void Show()
    {
        std::cout << 0 << std::endl;
    }

    static Obj0* Create(int n)
    {
        std::cout << n << std::endl;
        return new Obj0();
    }
};

class Obj1 : public ObjBase
{
public:
    virtual ~Obj1()
    {
        std::cout << "~Obj1" << std::endl;
    }
    virtual void Show()
    {
        std::cout << 1 << std::endl;
    }

    static Obj1* Create(int n)
    {
        std::cout << n << std::endl;
        return new Obj1();
    }
};

class Obj2 : public ObjBase
{
public:
    virtual ~Obj2()
    {
        std::cout << "~Obj2" << std::endl;
    }
    virtual void Show()
    {
        std::cout << 2 << std::endl;
    }

    static Obj2* Create1()
    {
        std::cout << "Create Obj2" << std::endl;
        return new Obj2();
    }

    static Obj2* Create2(int n)
    {
        std::cout << n << std::endl;
        return new Obj2();
    }
};

// using ObjStaticFactory = Arrow::Pattern::StaticFactory<ObjKeyEnum,
//                                                        ObjBase,
//                                                        Arrow::Pattern::StaticCreateObj<ObjKeyEnum, ObjKeyEnum::ObjKey0, Obj0>,
//                                                        Arrow::Pattern::StaticCreateObj<ObjKeyEnum, ObjKeyEnum::ObjKey1, Obj1>,
//                                                        Arrow::Pattern::StaticCreateObj<ObjKeyEnum, ObjKeyEnum::ObjKey2, Obj2>>;

using ObjStaticFactory = Arrow::Pattern::StaticFactory<ObjKeyEnum,
                                                       ObjBase,
                                                       std::integral_constant<ObjKeyEnum, ObjKeyEnum::ObjKey0>, Obj0,
                                                       std::integral_constant<ObjKeyEnum, ObjKeyEnum::ObjKey1>, Obj1,
                                                       std::integral_constant<ObjKeyEnum, ObjKeyEnum::ObjKey2>, Obj2>;

using ObjFactory1 = Arrow::Pattern::Singleton<Arrow::Pattern::Factory<ObjKeyEnum, ObjBase>>;
using ObjFactory2 = Arrow::Pattern::Singleton<Arrow::Pattern::Factory<ObjKeyEnum, ObjBase, int>>;

void FactoryDemo()
{
    ObjBase* pObj0 = ObjStaticFactory::Create(ObjKeyEnum::ObjKey0);
    ObjBase* pObj1 = ObjStaticFactory::Create(ObjKeyEnum::ObjKey1);
    ObjBase* pObj2 = ObjStaticFactory::Create(ObjKeyEnum::ObjKey2);
    pObj0->Show();
    pObj1->Show();
    pObj2->Show();

    delete pObj0;
    delete pObj1;
    delete pObj2;


    ObjFactory1::Instance()->Register<Obj0>(ObjKeyEnum::ObjKey0);
    ObjFactory1::Instance()->Register<Obj1>(ObjKeyEnum::ObjKey1, &Arrow::Other::CreateUseMalloc<Obj1>::Create);
    ObjFactory1::Instance()->Register<Obj2>(ObjKeyEnum::ObjKey2, &Obj2::Create1);
    
    pObj0 = ObjFactory1::Instance()->Create(ObjKeyEnum::ObjKey0);
    pObj1 = ObjFactory1::Instance()->Create(ObjKeyEnum::ObjKey1);
    pObj2 = ObjFactory1::Instance()->Create(ObjKeyEnum::ObjKey2);

    pObj0->Show();
    pObj1->Show();
    pObj2->Show();

    delete pObj0;
    delete pObj1;
    delete pObj2;

    ObjFactory2::Instance()->Register<Obj0>(ObjKeyEnum::ObjKey0, &Obj0::Create);
    ObjFactory2::Instance()->Register<Obj1>(ObjKeyEnum::ObjKey1, &Obj1::Create);
    ObjFactory2::Instance()->Register<Obj2>(ObjKeyEnum::ObjKey2, &Obj2::Create2);
    
    pObj0 = ObjFactory2::Instance()->Create(ObjKeyEnum::ObjKey0, 1);
    pObj1 = ObjFactory2::Instance()->Create(ObjKeyEnum::ObjKey1, 2);
    pObj2 = ObjFactory2::Instance()->Create(ObjKeyEnum::ObjKey2, 3);

    pObj0->Show();
    pObj1->Show();
    pObj2->Show();

    delete pObj0;
    delete pObj1;
    delete pObj2;
}