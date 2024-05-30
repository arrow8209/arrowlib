
#pragma once 
#include "../typelist_type.h"

namespace Arrow
{

// 在头部插入类型
template <typename T, typename typeList>
struct PushFront;

template <typename T, typename... Args>
struct PushFront<T, TypeList<Args...>>
{
    using type = TypeList<T, Args...> ;
};

template <typename T, typename typeList>
using PushFront_t = typename PushFront<T, typeList>::type;

}
