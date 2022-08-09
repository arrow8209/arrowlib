#pragma once
#include <map>
#include "typelist_type.h"
#include "typelist_fun.h"

namespace Arrow
{
namespace smap
{
template<typename Key, typename Value>
struct  static_pair;

template<>
struct  static_pair<std::nullptr_t, std::nullptr_t>
{
    typedef std::nullptr_t KeyType;
    typedef std::nullptr_t ValueType;
    constexpr static std::nullptr_t const key = nullptr;
    constexpr static std::nullptr_t const value = nullptr;
};
typedef static_pair<std::nullptr_t, std::nullptr_t> static_pair_null;

template<typename TKey, TKey Key, typename TValue, TValue Value>
struct static_pair<tvalue_type<TKey, Key>, tvalue_type<TValue, Value>>
{
    typedef TKey KeyType;
    typedef TValue ValueType;
    constexpr static TKey const key = Key;
    constexpr static TValue const value = Value;
};

template<typename TKey, TKey Key, typename T, T... args>
struct static_pair<tvalue_type<TKey, Key>, typelist<tvalue_type<T, args>...>>
{
    typedef TKey KeyType;
    typedef typename tlist::tvaluelist_to_data<typelist<tvalue_type<T, args>...>>::type ValueType;
    constexpr static TKey const key = Key;
    constexpr static ValueType* const value = tlist::tvaluelist_to_data<typelist<tvalue_type<T, args>...>>::data;
};

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