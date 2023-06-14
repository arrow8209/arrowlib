
#pragma once 
#include "../typelist_type.h"

namespace Arrow
{

namespace tlist
{

// 在头部插入类型
template <typename T, typename TList>
struct push_front;

template <typename T, typename... Args>
struct push_front<T, TypeList<Args...>>
{
    typedef TypeList<T, Args...> type;
};

}
}
