#pragma once
#include "../typelist_type.h"

namespace Arrow
{

// 在尾部插入类型
template <typename T, typename typeList>
struct PushBack;

template <typename T, typename... Args>
struct PushBack<T, TypeList<Args...>>
{
    using type = TypeList<Args..., T>;
};

template <typename T, typename typeList>
using PushBack_t = typename PushBack<T, typeList>::type;

}