#pragma once 
#include "../typelist_type.h"

namespace Arrow
{

namespace tlist
{

// 在尾部插入类型
template <typename T, typename TList>
struct push_back;

template <typename T, typename... Args>
struct push_back<T, TypeList<Args...>>
{
    typedef TypeList<Args..., T> type;
};
}
}