#pragma once
#include <iostream>
namespace Arrow
{

template<typename ...Args>
struct TypeList
{
    constexpr static unsigned int length = sizeof...(Args);
};

template<>
struct TypeList<>
{
    constexpr static unsigned int length = 0;
};

template <typename T, T t>
struct ValueType
{
    static_assert(!std::is_const<T>::value, "请勿使用const类型,主要是防止在进行查找的时候，值相等，但是类型不匹配的问题");
    static_assert(!std::is_volatile<T>::value, "请勿使用volatile类型,主要是防止在进行查找的时候，值相等，但是类型不匹配的问题");
    using type = T;
    constexpr static const type value = t;
};

namespace details
{
template <typename T, T ...args>
struct ValueTypeList
{
    static_assert(!std::is_const<T>::value, "请勿使用const类型,主要是防止在进行查找的时候，值相等，但是类型不匹配的问题");
    static_assert(!std::is_volatile<T>::value, "请勿使用volatile类型,主要是防止在进行查找的时候，值相等，但是类型不匹配的问题");
    using type=TypeList<ValueType<T, args>...>;
};

} // namespace details

template<typename T, T... args>
using ValueTypeList=typename details::ValueTypeList<T, args...>::type;

template<int N>
using IntValueType=ValueType<int, N>;

template<int... args>
using IntValueTypeList=typename details::ValueTypeList<int, args...>::type;

} // namespace name