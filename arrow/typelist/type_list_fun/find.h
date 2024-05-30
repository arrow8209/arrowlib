#pragma once
#include "../typelist_type.h"

namespace Arrow
{

// 查找指定类型第一次出现的位置 -1表示未出现
template <typename T, typename typeList>
struct Find;

template <typename T>
struct Find<T, TypeList<>>
{
    using type = TypeList<>;
    constexpr static int value = -1;
};

template <typename T, typename... Args>
struct Find<T, TypeList<T, Args...>>
{
    using type = T;
    constexpr static int value = 0;
};

template <typename T, typename Head, typename... Args>
struct Find<T, TypeList<Head, Args...>>
{
protected:
    using findNext = Find<T, TypeList<Args...>>;
    constexpr static int findNext_v = findNext::value;

public:
    constexpr static int value = findNext_v == -1 ? -1 : findNext_v + 1;
    using type = typename findNext::type;
};

// // C++11不支持，但是g++ 设置 std=C++11 能编译通过，但是会有警告 [zhuyb 2023-06-16 09:35:51]
// template<typename T, typename TList>
// static constexpr int Find_v= Find<T, TList>::value;

}