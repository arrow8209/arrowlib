#pragma once 
#include "../typelist_type.h"

namespace Arrow
{

// 拼接typelist
template<typename TList1, typename TList2>
struct Concat;

template<typename ...Args1, typename ...Args2>
struct Concat<TypeList<Args1...>, TypeList<Args2...>>
{
    using type=TypeList<Args1..., Args2...> ;
};

template<typename TList1, typename TList2>
using Concat_t = typename Concat<TList1, TList2>::type;

}