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
}