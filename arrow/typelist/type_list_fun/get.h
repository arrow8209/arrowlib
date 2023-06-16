#pragma once 
#include "../typelist_type.h"

namespace Arrow
{

// 获取指定下标的类型
template<int index, typename typeList>
struct Get;

template<typename T, typename ...Args>
struct Get<0, TypeList<T, Args...> >
{
    using type = T;
};

template<int index, typename T, typename ...Args>
struct Get<index, TypeList<T, Args...> >
{
    using type = typename Get<index-1, TypeList<Args...>>::type;
};

template<int index, typename typeList>
using Get_t = typename Get<index, typeList>::type;

}