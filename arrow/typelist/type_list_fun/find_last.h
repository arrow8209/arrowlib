#pragma once 
#include "../typelist_type.h"

namespace Arrow
{

namespace tlist
{

// 查找指定类型最后一次出现的位置 -1表示未出现
template<typename T, typename TList>
struct FindLast;

template <typename T>
struct FindLast<T, TypeList<>>
{
    constexpr static int value = -1;
    typedef TypeList<> type;
};

template<typename T, typename... Args>
struct FindLast<T, TypeList<T, Args...>>
{
protected:
    typedef FindLast<T, TypeList<Args...>> find_last_next;
    constexpr static int tmp = find_last_next::value;
public:
    constexpr static int value = tmp == -1 ? 0 : tmp + 1;
    typedef typename find_last_next::type type;
};

template <typename T, typename Head, typename... Args>
struct FindLast<T, TypeList<Head, Args...>>
{
protected:
    typedef FindLast<T, TypeList<Args...>> find_last_next;
    constexpr static int tmp = find_last_next::value;

public:
    constexpr static int value = tmp == -1 ? -1 : tmp + 1;
    typedef typename find_last_next::type type;
};
}
}