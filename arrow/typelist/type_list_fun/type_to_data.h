#pragma once 
#include "../typelist_type.h"
#include "push_front.h"

namespace Arrow
{

namespace tlist
{


#pragma region 值类型列表转对应数组
template<typename TList, typename T=void>
struct tvaluelist_to_data;

// 使用enable_if的写法主要是适配gcc 编译期，如果是 clange编译器，直接用特化处理就OK了 [zhuyb 2022-08-03 23:50:02]
template <typename T, T... args>
struct tvaluelist_to_data<TypeList<ValueType<T, args>...>, 
        typename std::enable_if<!std::is_same<T, char>::value, void>::type>
{
    typedef T type;
    constexpr static int Length = sizeof...(args);
    constexpr static T const data[sizeof...(args)] = {args...};
};
template<typename T, T ...args>
T const tvaluelist_to_data<TypeList<ValueType<T, args>...>, typename std::enable_if<!std::is_same<T, char>::value, void>::type>::data[sizeof...(args)];

template<typename T, T ...args>
struct tvaluelist_to_data<TypeList<ValueType<const T, args>...>, typename std::enable_if<!std::is_same<T, char>::value, void>::type>
{
    typedef T type;
    constexpr static int Length = sizeof...(args);
    constexpr static T const data[sizeof...(args)] = {args...};
};
template<typename T, T ...args>
const T tvaluelist_to_data<TypeList<ValueType<const T, args>...>,typename std::enable_if<!std::is_same<T, char>::value, void>::type>::data[sizeof...(args)];

template<const char ...args>
struct tvaluelist_to_data<TypeList<ValueType<char, args>...>, void>
{
    typedef char type;
    constexpr static int Length = sizeof...(args) + 1;
    constexpr static char const data[sizeof...(args) + 1] = {args..., 0};
};
template<char ...args>
char const tvaluelist_to_data<TypeList<ValueType<char, args>...>, void>::data[sizeof...(args) + 1];

template<const char ...args>
struct tvaluelist_to_data<TypeList<ValueType<const char, args>...>, void>
{
    typedef char type;
    constexpr static int Length = sizeof...(args) + 1;
    constexpr static char const data[sizeof...(args) + 1] = {args..., 0};
};
template <char... args>
char const tvaluelist_to_data<TypeList<ValueType<const char, args>...>, void>::data[sizeof...(args) + 1];

}
}