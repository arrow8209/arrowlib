/*
 * @FilePath: /arrowlib/arrow/typelist/static_map.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-08-10 08:20:07
 * @Description: 静态（编译期） map
 */
#pragma once
#include <map>
#include "typelist_type.h"
#include "typelist_fun.h"

namespace Arrow
{
namespace smap
{

#pragma region static_pair

// static_map 的存储类型 [zhuyb 2022-09-14 17:51:33]
template<typename Key, typename Value>
struct  static_pair;

// 用于表示结束类型 [zhuyb 2022-09-14 17:51:52]
template<>
struct  static_pair<std::nullptr_t, std::nullptr_t>
{
    typedef std::nullptr_t KeyType;
    typedef std::nullptr_t ValueType;
    constexpr static std::nullptr_t const key = nullptr;
    constexpr static std::nullptr_t const value = nullptr;
};
typedef static_pair<std::nullptr_t, std::nullptr_t> static_pair_null;

// 普遍类型的static_pair [zhuyb 2022-09-14 17:52:21]
template<typename TKey, TKey Key, typename TValue, TValue Value>
struct static_pair<tvalue_type<TKey, Key>, tvalue_type<TValue, Value>>
{
    typedef TKey KeyType;
    typedef TValue ValueType;
    static constexpr TKey key = Key;
    static constexpr TValue value = Value;
};

template <typename TKey, TKey Key, typename TValue, TValue Value>
constexpr TKey static_pair<tvalue_type<TKey, Key>, tvalue_type<TValue, Value>>::key;

template <typename TKey, TKey Key, typename TValue, TValue Value>
constexpr TValue static_pair<tvalue_type<TKey, Key>, tvalue_type<TValue, Value>>::value;

// staic_pair 存放的数组（例如static_string） [zhuyb 2022-09-14 17:57:26]
template<typename TKey, TKey Key, typename T, T... args>
struct static_pair<tvalue_type<TKey, Key>, typelist<tvalue_type<T, args>...>>
{
    typedef TKey KeyType;
    typedef typename tlist::tvaluelist_to_data<typelist<tvalue_type<T, args>...>>::type ValueType;
    static constexpr TKey key = Key;
    static constexpr const ValueType* value = tlist::tvaluelist_to_data<typelist<tvalue_type<T, args>...>>::data;
};
template <typename TKey, TKey Key, typename T, T... args>
constexpr TKey static_pair<tvalue_type<TKey, Key>, typelist<tvalue_type<T, args>...>>::key;

template <typename TKey, TKey Key, typename T, T... args>
constexpr const typename static_pair<tvalue_type<TKey, Key>, typelist<tvalue_type<T, args>...>>::ValueType* 
    static_pair<tvalue_type<TKey, Key>, typelist<tvalue_type<T, args>...>>::value;

#pragma endregion

#pragma region find

// 查找staic_pair [zhuyb 2022-09-14 18:06:52]
template<typename T, typename TPairList>
struct find;

template<typename Key>
struct find<Key, typelist<>>
{
    typedef static_pair_null Pair;
    typedef static_pair_null Tail;
};

template<typename Key, typename Value, typename ...Args>
struct find<Key, typelist<static_pair<Key, Value>, Args...>>
{
    typedef static_pair<Key, Value> Pair;
    typedef typelist<Args...> Tail;
};

template<typename T, typename Key, typename Value, typename ...Args>
struct find<T, typelist<static_pair<Key, Value>, Args...>>
{
    typedef typename find<T, typelist<Args...>>::Pair Pair;
    typedef typename find<T, typelist<Args...>>::Tail Tail;
};
#pragma endregion

#pragma region static_map


template <typename... Args>
struct static_map;

template<typename Key, typename Value>
struct static_map<static_pair<Key, Value>>
{
    typedef typelist<static_pair<Key, Value>> type;
};


template <typename Key, typename Value, typename... Args>
struct static_map<static_pair<Key, Value>, Args...>
{
public:
    // 对后面的数据处理 [zhuyb 2022-08-08 23:45:25]
    typedef typename static_map<Args...>::type tail;

    // 查找后面有没有重复的key [zhuyb 2022-08-08 23:44:38]
    typedef typename find<Key, typelist<Args...>>::Pair Pair;

    // 没有重复的key [zhuyb 2022-08-08 23:44:58]
    typedef typename tlist::push_front<static_pair<Key, Value>, tail>::type one_key;

    // 有重复的key [zhuyb 2022-08-08 23:45:10]
    typedef tail two_key;

public:
    typedef typename enable_ternary<std::is_same<Pair, static_pair_null>::value, one_key, two_key>::type type;

};

#pragma endregion

template<typename T, typename TPairList>
struct get;

template<typename Key, typename ...Args>
struct get<Key, static_map<Args...>>
{
    typedef typename find<Key, typename static_map<Args...>::type>::Pair Pair;
    typedef typename find<Key, typename static_map<Args...>::type>::Tail Tail;
};

}

template<typename Key, typename Value>
using  static_pair = smap::static_pair<Key, Value>;

template <typename ...Args>
using static_map = smap::static_map<Args...>;



namespace tlist
{

void print(smap::static_pair_null)
{
    std::cout<< "(nullptr, nullptr)" << std::endl;
}

template <typename Key, typename Value>
void print(static_pair<Key, Value>)
{
    std::cout<< "("<< static_pair<Key, Value>::key << "," << static_pair<Key, Value>::value << ")" << std::endl;
}

template <typename Key, typename Value, typename ...Args>
void print(typelist<static_pair<Key, Value>, Args...>)
{
    print(static_pair<Key, Value>{});
    std::cout << "\t";
    print(typelist<Args...>{});
}

template <typename... Args>
void print(static_map<Args...>)
{
    print(typename static_map<Args...>::type{});
}

}
}