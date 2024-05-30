#pragma once 
#include "../typelist_type.h"
#include "push_front.h"

namespace Arrow
{

// 移除指定下标的类型
template<int index, typename typeList>
struct Remove;

template<typename T, typename ...Args>
struct Remove<0, TypeList<T, Args...>>
{
    using type = TypeList<Args...>;
    using remove_type = T;
};

template<int index, typename T, typename ...Args>
struct Remove<index, TypeList<T, Args...>>
{
protected:
    using removeNext = TypeList<Args...>;
    using inputType = TypeList<T, Args...>;
    using result = Remove<index - 1, removeNext>;

    static_assert(index >= 0, "输入的index编号，必须 >= 0");
    static_assert(index < inputType::length, "输入的index编号，超出类型列表长度");

public:
    using type = PushFront_t<T, typename result::type>;
    using remove_type = typename result::remove_type ;
};

template<int index, typename typeList>
using Remove_t = typename Remove<index, typeList>::type;

}