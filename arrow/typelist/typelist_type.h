#pragma once
#include <iostream>
namespace Arrow
{
namespace typelist
{
template<typename ...Args>
struct typelist
{
    constexpr static unsigned int length = sizeof...(Args);
};

template<>
struct typelist<>
{
    constexpr static unsigned int length = 0;
};

// 注意此处尽量不要使用const类型，主要是担心在使用 typelist功能函数的时候，const类型和标准类型会匹配不上的问题[zhuyb 2022-08-02 09:53:41]
// template <typename T, T t>
// struct tvalue_type
// {
//     typedef typename std::remove_const<typename std::remove_cv<typename std::remove_volatile<T>::type>::type>::type type;
//     constexpr static const type value = t;
// };

template <typename T, T t>
struct tvalue_type
{
    typedef typename std::remove_cv<T>::type type;
    constexpr static const type value = t;
};

// template <typename T, T t>
// struct tvalue_type<typename std::remove_cv<T>::type, t>
// {
//     typedef typename std::remove_cv<T>::type type;
//     constexpr static const type value = t;
// };

namespace details
{

template <int n>
struct value_type
{
    typedef int type;
    constexpr static const int value = n;
};

template <int... args>
struct value_typelist
{
    typedef typelist<tvalue_type<int, args>...> type;
};


template <typename T, T... args>
struct tvalue_typelist
{
    typedef typelist<tvalue_type<T, args>...> type;
    constexpr static std::size_t length = sizeof...(args);
};

} // namespace details

template<int N>
using value_type=typename details::value_type<N>::type;

template<int... args>
using value_typelist=typename details::value_typelist<args...>::type;

template<typename T, T... args>
using tvalue_typelist=typename details::tvalue_typelist<T, args...>::type;

}

} // namespace name