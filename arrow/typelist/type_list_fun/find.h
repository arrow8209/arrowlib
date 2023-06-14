#pragma once 
#include "../typelist_type.h"

namespace Arrow
{

namespace tlist
{

// 查找指定类型第一次出现的位置 -1表示未出现 
template <typename T, typename TList>
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
    typedef Find<T, TypeList<Args...>> find_tmp;
    constexpr static int tmp = find_tmp::value;

public:
    constexpr static int value = tmp == -1 ? -1 : tmp + 1;
    typedef typename find_tmp::type type;
};
}
}