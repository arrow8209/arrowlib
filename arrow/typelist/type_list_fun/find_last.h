#pragma once 
#include "../typelist_type.h"

namespace Arrow
{

// 查找指定类型最后一次出现的位置 -1表示未出现
template<typename T, typename typeList>
struct FindLast;

template <typename T>
struct FindLast<T, TypeList<>>
{
    constexpr static int value = -1;
    using type = TypeList<>;
};

template <typename T, typename... Args>
struct FindLast<T, TypeList<T, Args...>>
{
protected:
    using findLastNext = FindLast<T, TypeList<Args...>>;
    constexpr static int findLastNext_v = findLastNext::value;

public:
    using type = typename findLastNext::type;
    constexpr static int value = findLastNext_v == -1 ? 0 : findLastNext_v + 1;
};

template <typename T, typename Head, typename... Args>
struct FindLast<T, TypeList<Head, Args...>>
{
protected:
    using findLastNext = FindLast<T, TypeList<Args...>>;
    constexpr static int findLastNext_v = findLastNext::value;

public:
    using type = typename findLastNext::type;
    constexpr static int value = findLastNext_v == -1 ? -1 : findLastNext_v + 1;
};

template<typename T, typename TList>
using FindLast_t= typename FindLast<T, TList>::type;

// C++11不支持，但是g++ 设置 std=C++11 能编译通过，但是会有警告 [zhuyb 2023-06-16 09:35:51]
template<typename T, typename TList>
static constexpr int FindLast_v= FindLast<T, TList>::value;

}