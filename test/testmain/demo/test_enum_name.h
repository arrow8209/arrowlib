#pragma once
#include <iostream>
#include <map>
#include <string>
#include <iostream>
#include "arrow/arrow.h"
#include "arrow/typelist/type_name/enum_name_impl.h"

enum class MyEnumA
{
    MyEnumAItem01 = 0,
    MyEnumAItem02,
    MyEnumAItem03,
    MyEnumAItem10 = 10,
    MyEnumAItem11,
    MyEnumAItem12
};
using MyEnumAStr = Arrow::EnumListToStr<MyEnumA, MyEnumA::MyEnumAItem01, MyEnumA::MyEnumAItem03, MyEnumA::MyEnumAItem10, MyEnumA::MyEnumAItem12>;

enum MyEnumB
{
    MyEnumBItem01 = 0,
    MyEnumBItem02,
    MyEnumBItem03,
    MyEnumBItem10 = 10,
    MyEnumBItem11,
    MyEnumBItem12
};
using MyEnumBStr = Arrow::EnumListToStr<MyEnumB, MyEnumB::MyEnumBItem01, MyEnumB::MyEnumBItem03, MyEnumB::MyEnumBItem10, MyEnumB::MyEnumBItem12>;

namespace MyNameSpace
{

enum MyEnumC
{
    MyEnumCItem0 = 0,
    E12345678
};
using MyEnumCStr = Arrow::EnumListToStr<MyEnumC, MyEnumC::MyEnumCItem0, MyEnumC::E12345678>;


enum class MyEnumD
{
    MyEnumDItem0 = 0,
    E12345679
} ;
using MyEnumDStr = Arrow::EnumListToStr<MyEnumD, MyEnumD::MyEnumDItem0, MyEnumD::E12345679>;

}

static void TestEnumName()
{
    std::cout << Arrow::EnumItemName<MyEnumA, MyEnumA::MyEnumAItem01>::Trace3() << std::endl;
    std::cout << __VERSION__ << std::endl;
    std::cout << MyEnumAStr::ItemStr(MyEnumA::MyEnumAItem01) << std::endl;
    std::cout << MyEnumAStr::ItemStr(MyEnumA::MyEnumAItem02) << std::endl;
    std::cout << MyEnumAStr::ItemStr(MyEnumA::MyEnumAItem03) << std::endl;
    std::cout << MyEnumAStr::ItemStr(MyEnumA::MyEnumAItem10) << std::endl;
    std::cout << MyEnumAStr::ItemStr(MyEnumA::MyEnumAItem11) << std::endl;
    std::cout << MyEnumAStr::ItemStr(MyEnumA::MyEnumAItem12) << std::endl;

    std::cout << MyEnumBStr::ItemStr(MyEnumB::MyEnumBItem01) << std::endl;
    std::cout << MyEnumBStr::ItemStr(MyEnumB::MyEnumBItem02) << std::endl;
    std::cout << MyEnumBStr::ItemStr(MyEnumB::MyEnumBItem03) << std::endl;
    std::cout << MyEnumBStr::ItemStr(MyEnumB::MyEnumBItem10) << std::endl;
    std::cout << MyEnumBStr::ItemStr(MyEnumB::MyEnumBItem11) << std::endl;
    std::cout << MyEnumBStr::ItemStr(MyEnumB::MyEnumBItem12) << std::endl;

    std::cout << MyNameSpace::MyEnumCStr::ItemStr(MyNameSpace::MyEnumC::MyEnumCItem0) << std::endl;
    std::cout << MyNameSpace::MyEnumCStr::ItemStr(MyNameSpace::MyEnumC::E12345678) << std::endl;

    std::cout << MyNameSpace::MyEnumDStr::ItemStr(MyNameSpace::MyEnumD::MyEnumDItem0) << std::endl;
    std::cout << MyNameSpace::MyEnumDStr::ItemStr(MyNameSpace::MyEnumD::E12345679) << std::endl;
}

