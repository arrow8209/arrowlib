#pragma once
#include "../typelist_type.h"
#include "push_front.h"

namespace Arrow
{

// 值类型列表转对应数组
template <typename typeList, typename SFINA = void>
struct ValueListToData;

// 使用enable_if的写法主要是适配gcc 编译期，如果是 clange编译器，直接用特化处理就OK了 [zhuyb 2022-08-03 23:50:02]
// 非字符类型 [zhuyb 2023-06-16 10:56:13]
template <typename T, T... args>
struct ValueListToData<TypeList<ValueType<T, args>...>,
                       typename std::enable_if<!std::is_same<T, char>::value, void>::type>
{
    using type = T;
    constexpr static int length = sizeof...(args);
    constexpr static T const data[sizeof...(args)] = {args...};
};
template <typename T, T... args>
T const ValueListToData<TypeList<ValueType<T, args>...>, typename std::enable_if<!std::is_same<T, char>::value, void>::type>::data[sizeof...(args)];

// template <typename T, T... args>
// struct ValueListToData<TypeList<ValueType<const T, args>...>, typename std::enable_if<!std::is_same<T, char>::value, void>::type>
// {
//     typedef T type;
//     constexpr static int length = sizeof...(args);
//     constexpr static T const data[sizeof...(args)] = {args...};
// };
// template <typename T, T... args>
// const T ValueListToData<TypeList<ValueType<const T, args>...>, typename std::enable_if<!std::is_same<T, char>::value, void>::type>::data[sizeof...(args)];

template <const char... args>
struct ValueListToData<TypeList<ValueType<char, args>...>, void>
{
    typedef char type;
    constexpr static int length = sizeof...(args) + 1;
    constexpr static char const data[sizeof...(args) + 1] = {args..., 0};
};
template <char... args>
char const ValueListToData<TypeList<ValueType<char, args>...>, void>::data[sizeof...(args) + 1];

// template <const char... args>
// struct ValueListToData<TypeList<ValueType<const char, args>...>, void>
// {
//     typedef char type;
//     constexpr static int length = sizeof...(args) + 1;
//     constexpr static char const data[sizeof...(args) + 1] = {args..., 0};
// };
// template <char... args>
// char const ValueListToData<TypeList<ValueType<const char, args>...>, void>::data[sizeof...(args) + 1];

}