#pragma once
#include <type_traits>
#include "typelist_type.h"
#include "make_integer_sequence.h"
#include "static_string2.h"
#include <iostream>

namespace Arrow
{

template<typename T>
struct TypeName2
{
    static std::string Name()
    {
        constexpr auto str = __PRETTY_FUNCTION__;
        std::cout << str << std::endl;
        constexpr int start = Arrow::StaticStr::Find(str, '=');
        constexpr int end = Arrow::StaticStr::Find(str, ';');
        constexpr auto subStr = Arrow::StaticStr::SubStr<start + 2, end-start - 2>(str);
        return subStr.data;
    }
};



}