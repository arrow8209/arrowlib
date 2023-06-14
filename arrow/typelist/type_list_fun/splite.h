
#pragma once 
#include "../typelist_type.h"
#include "push_front.h"

namespace Arrow
{

namespace tlist
{

// 分割typelist遵循左开右闭原则
template<int Index, typename TList>
struct splite;

template<typename T, typename ...Args>
struct splite<1, TypeList<T, Args...>>
{
    typedef TypeList<T> Head;
    typedef TypeList<Args...> Tail;
};

template<int Count, typename T, typename ...Args>
struct splite<Count, TypeList<T, Args...>>
{
protected:
    typedef splite<Count - 1, TypeList<Args...>> splite_tmp;
    static_assert(Count > 0, "Count 必须 >0");
    static_assert(Count <= sizeof...(Args), "Count 必须小于列表的总长度 -1 ");
public:
    typedef typename push_front<T, typename splite_tmp::Head>::type Head;
    typedef typename splite_tmp::Tail Tail;
};
}
}