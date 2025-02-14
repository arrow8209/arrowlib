#pragma once
#include <iostream>
#include <type_traits>
#include "typelist_type.h"

namespace Arrow
{

namespace tlist
{

#pragma region 获取指定下标的类型
template<int index, typename TList>
struct get;

template<typename T, typename ...Args>
struct get<0, typelist<T, Args...> >
{
    typedef T type;
};

template<int index, typename T, typename ...Args>
struct get<index, typelist<T, Args...> >
{
    typedef typename get<index-1, typelist<Args...>>::type type;
};

#pragma endregion

#pragma region 查找指定类型第一次出现的位置 -1表示未出现 
template <typename T, typename TList>
struct find;

template <typename T>
struct find<T, typelist<>>
{
    constexpr static int value = -1;
    typedef typelist<> type;
};

template <typename T, typename... Args>
struct find<T, typelist<T, Args...>>
{
    constexpr static int value = 0;
    typedef T type;
};

template <typename T, typename Head, typename... Args>
struct find<T, typelist<Head, Args...>>
{
protected:
    typedef find<T, typelist<Args...>> find_tmp;
    constexpr static int tmp = find_tmp::value;

public:
    constexpr static int value = tmp == -1 ? -1 : tmp + 1;
    typedef typename find_tmp::type type;
};

#pragma endregion

#pragma region 查找指定类型最后一次出现的位置 -1表示未出现
template<typename T, typename TList>
struct find_last;

template <typename T>
struct find_last<T, typelist<>>
{
    constexpr static int value = -1;
    typedef typelist<> type;
};

template<typename T, typename... Args>
struct find_last<T, typelist<T, Args...>>
{
protected:
    typedef find_last<T, typelist<Args...>> find_last_next;
    constexpr static int tmp = find_last_next::value;
public:
    constexpr static int value = tmp == -1 ? 0 : tmp + 1;
    typedef typename find_last_next::type type;
};

template <typename T, typename Head, typename... Args>
struct find_last<T, typelist<Head, Args...>>
{
protected:
    typedef find_last<T, typelist<Args...>> find_last_next;
    constexpr static int tmp = find_last_next::value;

public:
    constexpr static int value = tmp == -1 ? -1 : tmp + 1;
    typedef typename find_last_next::type type;
};

#pragma endregion

#pragma region 拼接typelist
template<typename TList1, typename TList2>
struct concat;

template<typename ...Args1, typename ...Args2>
struct concat<typelist<Args1...>, typelist<Args2...>>
{
    typedef typelist<Args1..., Args2...> type;
};


#pragma endregion

#pragma region 在尾部插入类型
template <typename T, typename TList>
struct push_back;

template <typename T, typename... Args>
struct push_back<T, typelist<Args...>>
{
    typedef typelist<Args..., T> type;
};

#pragma endregion

#pragma region 在头部插入类型
template <typename T, typename TList>
struct push_front;

template <typename T, typename... Args>
struct push_front<T, typelist<Args...>>
{
    typedef typelist<T, Args...> type;
};

#pragma endregion

#pragma region 移除指定下标的类型
template<int index, typename TList>
struct remove_at;

template<typename T, typename ...Args>
struct remove_at<0, typelist<T, Args...>>
{
    typedef typelist<Args...> type;
    typedef T remove_type;
};

template<int index, typename T, typename ...Args>
struct remove_at<index, typelist<T, Args...>>
{
protected:
    typedef typelist<Args...> typelist_type_next;
    typedef typelist<T, Args...> typelist_type_input;
    static_assert(index >=0 , "输入的index编号，必须 >= 0");
    static_assert(index < typelist_type_input::length, "输入的index编号，超出类型列表长度");

    typedef remove_at<index - 1, typelist_type_next> result;
public:
    typedef typename push_front<T, typename result::type>::type type;
    typedef typename result::remove_type remove_type;
};

#pragma endregion

#pragma region 移除尾部类型
template <typename TList>
struct pop_back;

template <typename... Args>
struct pop_back<typelist<Args...>>
{
protected:
    typedef typelist<Args...> TList;
    typedef remove_at<TList::length - 1, TList> remove_tmp;

public:
    typedef typename remove_tmp::remove_type Tail;
    typedef typename remove_tmp::type type;
};

template <>
struct pop_back<typelist<>>
{
    typedef typelist<> Tail;
    typedef typelist<> type;
};

#pragma endregion

#pragma region 移除头部类型
template <typename TList>
struct pop_front;

template <typename T, typename... Args>
struct pop_front<typelist<T, Args...>>
{
    typedef T Head;
    typedef typelist<Args...> type;
};

template <>
struct pop_front<typelist<>>
{
    typedef typelist<> Head;
    typedef typelist<> type;
};
#pragma endregion

#pragma region 分割typelist遵循左开右闭原则
template<int Index, typename TList>
struct splite;

template<typename T, typename ...Args>
struct splite<1, typelist<T, Args...>>
{
    typedef typelist<T> Head;
    typedef typelist<Args...> Tail;
};

template<int Count, typename T, typename ...Args>
struct splite<Count, typelist<T, Args...>>
{
protected:
    typedef splite<Count - 1, typelist<Args...>> splite_tmp;
    static_assert(Count > 0, "Count 必须 >0");
    static_assert(Count <= sizeof...(Args), "Count 必须小于列表的总长度 -1 ");
public:
    typedef typename push_front<T, typename splite_tmp::Head>::type Head;
    typedef typename splite_tmp::Tail Tail;
};
#pragma endregion

#pragma region 值类型列表转对应数组
template<typename TList, typename T=void>
struct tvaluelist_to_data;

// 使用enable_if的写法主要是适配gcc 编译期，如果是 clange编译器，直接用特化处理就OK了 [zhuyb 2022-08-03 23:50:02]
template <typename T, T... args>
struct tvaluelist_to_data<typelist<tvalue_type<T, args>...>, 
        typename std::enable_if<!std::is_same<T, char>::value, void>::type>
{
    typedef T type;
    constexpr static int Length = sizeof...(args);
    constexpr static T const data[sizeof...(args)] = {args...};
};
template<typename T, T ...args>
T const tvaluelist_to_data<typelist<tvalue_type<T, args>...>, typename std::enable_if<!std::is_same<T, char>::value, void>::type>::data[sizeof...(args)];

template<typename T, T ...args>
struct tvaluelist_to_data<typelist<tvalue_type<const T, args>...>, typename std::enable_if<!std::is_same<T, char>::value, void>::type>
{
    typedef T type;
    constexpr static int Length = sizeof...(args);
    constexpr static T const data[sizeof...(args)] = {args...};
};
template<typename T, T ...args>
const T tvaluelist_to_data<typelist<tvalue_type<const T, args>...>,typename std::enable_if<!std::is_same<T, char>::value, void>::type>::data[sizeof...(args)];

template<const char ...args>
struct tvaluelist_to_data<typelist<tvalue_type<char, args>...>, void>
{
    typedef char type;
    constexpr static int Length = sizeof...(args) + 1;
    constexpr static char const data[sizeof...(args) + 1] = {args..., 0};
};
template<char ...args>
char const tvaluelist_to_data<typelist<tvalue_type<char, args>...>, void>::data[sizeof...(args) + 1];

template<const char ...args>
struct tvaluelist_to_data<typelist<tvalue_type<const char, args>...>, void>
{
    typedef char type;
    constexpr static int Length = sizeof...(args) + 1;
    constexpr static char const data[sizeof...(args) + 1] = {args..., 0};
};
template <char... args>
char const tvaluelist_to_data<typelist<tvalue_type<const char, args>...>, void>::data[sizeof...(args) + 1];

#pragma endregion

#pragma region 打印

static void print(typelist<>)
{
    std::cout << std::endl;
}

template<typename T, T t, T ...args>
static void print(typelist<tvalue_type<T,t>, tvalue_type<T, args>...>)
{
    std::cout << typeid(tvalue_type<T, t>).name() << ":" << t << "\n\t";
    print(typelist<tvalue_type<T, args>...>{});
}

template<typename T, T t, T ...args>
static void print(typelist<tvalue_type<const T,t>, tvalue_type<const T, args>...>)
{
    std::cout << typeid(tvalue_type<const T, t>).name() << ":" << t << "\n\t";
    print(typelist<tvalue_type<const T, args>...>{});
}

template<typename T, typename... args>
static void print(typelist<T, args...>)
{
    std::cout << typeid(T).name() << "\n\t";
    print(typelist<args...>{});
}

#pragma endregion

}
} // namespace  Arrow
