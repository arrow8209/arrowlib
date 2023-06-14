#pragma once
#include <iostream>
#include <type_traits>
#include "typelist_type.h"
#include "type_list_fun/concat.h"
#include "type_list_fun/find_last.h"
#include "type_list_fun/find.h"
#include "type_list_fun/get.h"
#include "type_list_fun/pop_front.h"
#include "type_list_fun/pop_back.h"
#include "type_list_fun/push_back.h"
#include "type_list_fun/push_front.h"
#include "type_list_fun/remove.h"
#include "type_list_fun/splite.h"
#include "type_list_fun/type_to_data.h"

namespace Arrow
{


namespace tlist
{

// 打印
static void print(TypeList<>)
{
    std::cout << std::endl;
}

template<typename T, T t, T ...args>
static void print(TypeList<ValueType<T,t>, ValueType<T, args>...>)
{
    std::cout << typeid(ValueType<T, t>).name() << ":" << t << "\n\t";
    print(TypeList<ValueType<T, args>...>{});
}

template<typename T, T t, T ...args>
static void print(TypeList<ValueType<const T,t>, ValueType<const T, args>...>)
{
    std::cout << typeid(ValueType<const T, t>).name() << ":" << t << "\n\t";
    print(TypeList<ValueType<const T, args>...>{});
}

template<typename T, typename... args>
static void print(TypeList<T, args...>)
{
    std::cout << typeid(T).name() << "\n\t";
    print(TypeList<args...>{});
}

}
} // namespace  Arrow
