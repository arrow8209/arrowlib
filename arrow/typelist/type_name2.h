#pragma once
#include <type_traits>
#include "typelist_type.h"
#include "make_integer_sequence.h"
#include <iostream>

namespace Arrow
{


// template<typename T>
// struct TypeName2
// {
//     static const char*&& Name()
//     {
//         constexpr auto str = __PRETTY_FUNCTION__;
//         std::cout << str << std::endl;
//         constexpr int start = Arrow::StaticStr::Find(str, '=');
//         constexpr int end = Arrow::StaticStr::Find(str, ']');
//         constexpr auto subStr = Arrow::StaticStr::SubStr<start + 2, end-start>(str);
//         return std::move(subStr.data);
//     }
// };

// template<typename T>
// constexpr const char* TypeNameFun(T t)
// {
//     return Arrow::StaticStr::SubStr<Arrow::StaticStr::Find(__PRETTY_FUNCTION__, '=') + 2, Arrow::StaticStr::Find(__PRETTY_FUNCTION__, ']') - Arrow::StaticStr::Find(__PRETTY_FUNCTION__, '=')>(__PRETTY_FUNCTION__).data;
// }

}