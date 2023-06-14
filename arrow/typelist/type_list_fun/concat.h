#pragma once 
#include "../typelist_type.h"

namespace Arrow
{

namespace tlist
{
    
// 拼接typelist
template<typename TList1, typename TList2>
struct concat;

template<typename ...Args1, typename ...Args2>
struct concat<TypeList<Args1...>, TypeList<Args2...>>
{
    typedef TypeList<Args1..., Args2...> type;
};

}
}