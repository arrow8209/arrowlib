#pragma once
#include <iostream>
#include <map>
#include <string>
#include <iostream>
#include "arrow/typelist/type_name.h"
#include "arrow/typelist/type_name/enum_name_impl.h"

enum TestEnum
{
    EmItem1 = 0,
    EmItem2,
    EmItem3,
    EmItemA = 10,
    EmItemB,
    EmItemC
};

static void TestTypeName()
{
    using typeName1=Arrow::TypeName<std::map<int, std::string>>;
    typeName1::Trace();
    std::cout << typeName1::Name() << std::endl;

    std::map<int, std::string> tmp;
    std::cout << Arrow::GetTypeName(tmp) <<std::endl;
}

static void TestEnumName()
{ 

    using enumName1=Arrow::EnumItemName<TestEnum, EmItemA>;
    enumName1::Trace();
    std::cout << enumName1::Name() << std::endl;
    std::cout << enumName1::FullName() << std::endl;

    using TestEnumListStr = Arrow::EnumListToStr<TestEnum, EmItem1, EmItem3, EmItemA, EmItemC>;
    std::cout << TestEnumListStr::ItemStr(EmItem1) << std::endl;
    std::cout << TestEnumListStr::ItemStr(EmItemC) << std::endl;
}