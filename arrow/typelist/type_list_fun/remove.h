#pragma once 
#include "../typelist_type.h"
#include "push_front.h"

// 貌似有问题 [zhuyb 2023-06-14 22:22:39]
namespace Arrow
{

namespace tlist
{
// 移除指定下标的类型
template<int index, typename TList>
struct remove_at;

template<typename T, typename ...Args>
struct remove_at<0, TypeList<T, Args...>>
{
    typedef TypeList<Args...> type;
    typedef T remove_type;
};

template<int index, typename T, typename ...Args>
struct remove_at<index, TypeList<T, Args...>>
{
protected:
    typedef TypeList<Args...> typelist_type_next;
    typedef TypeList<T, Args...> typelist_type_input;
    static_assert(index >=0 , "输入的index编号，必须 >= 0");
    static_assert(index < typelist_type_input::length, "输入的index编号，超出类型列表长度");

    typedef remove_at<index - 1, typelist_type_next> result;
public:
    typedef typename push_front<T, typename result::type>::type type;
    typedef typename result::remove_type remove_type;
};

}
}