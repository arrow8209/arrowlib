#pragma once
#include <map>
#include "typelist_type.h"
#include "typelist_fun.h"

namespace Arrow
{
namespace static_map
{

template<typename Key, typename Value>
struct  static_pair;

template<>
struct  static_pair<std::nullptr_t, std::nullptr_t>
{
    typedef std::nullptr_t KeyType;
    typedef std::nullptr_t ValueType;
    constexpr static std::nullptr_t const Key = nullptr;
    constexpr static std::nullptr_t const Value = nullptr;
};
typedef static_pair<std::nullptr_t, std::nullptr_t> static_pair_null;

template<typename TKey, TKey key, typename TValue, TValue value>
struct static_pair<typelist::tvalue_type<TKey, key>, typelist::tvalue_type<TValue, value>>
{
    typedef TKey KeyType;
    typedef TValue ValueType;
    constexpr static TKey const Key = key;
    constexpr static TValue const Value = value;
};

template<typename TKey, TKey key, typename TList>
struct static_pair<typelist::tvalue_type<TKey, key>, typelist::tvaluelist_to_data<TList>>
{
    typedef TKey KeyType;
    typedef typename typelist::tvaluelist_to_data<TList>::type ValueType;
    constexpr static TKey const Key = key;
    constexpr static ValueType* const Value = typelist::tvaluelist_to_data<TList>::data;
};

template<typename T, typename TPairList>
struct find;

template<typename Key>
struct find<Key, typelist::typelist<>>
{
    typedef static_pair_null Pair;
    typedef static_pair_null Tail;
};

template<typename Key, typename Value, typename ...Args>
struct find<Key, typelist::typelist<static_pair<Key, Value>, Args...>>
{
    typedef static_pair<Key, Value> Pair;
    typedef typename typelist::typelist<Args...> Tail;
};

template<typename T, typename Key, typename Value, typename ...Args>
struct find<T, typelist::typelist<static_pair<Key, Value>, Args...>>
{
    typedef typename find<T, typelist::typelist<Args...>>::Pair Pair;
    typedef typename find<T, typelist::typelist<Args...>>::Tail Tail;
};



template <typename TList>
struct static_map;

template<typename Key, typename Value>
struct static_map<typelist::typelist<static_pair<Key, Value>>>
{
    typedef typename typelist::typelist<static_pair<Key, Value>> type;
};


template <typename Key, typename Value, typename... Args>
struct static_map<typelist::typelist<static_pair<Key, Value>, Args...>>
{
public:
    // 对后面的数据处理 [zhuyb 2022-08-08 23:45:25]
    typedef typename static_map<typelist::typelist<Args...>>::type tail;

    // 查找后面有没有重复的key [zhuyb 2022-08-08 23:44:38]
    typedef typename find<Key, typelist::typelist<Args...>>::Pair Pair;

    // 没有重复的key [zhuyb 2022-08-08 23:44:58]
    typedef typename typelist::push_front<static_pair<Key, Value>, tail>::type one_key;

    // 有重复的key [zhuyb 2022-08-08 23:45:10]
    typedef tail two_key;

public:
    typedef typename enable_ternary<std::is_same<Pair, static_pair_null>::value, one_key, two_key>::type type;

};

// namespace details
// {
// template <typename... Args>
// struct static_map_data;

// template<typename Key, typename Value>
// static static_map_data<typelist::typelist<static_pair<Key, Value>>
// {
//     typedef typelist::typelist<static_pair<Key, Value> type;
//     constexpr static int Lenght = 1;
// };

// template<typename

// } // namespace de




// template<typename TKey, typename MapData>
// struct static_map;

// template<typename TKey>
// struct static_map<TKey, typelist::typelist<>>
// {
//     typedef std::nullptr_t KeyType;
//     typedef std::nullptr_t ValueType;
//     constexpr static KeyType = nullptr;
//     constexpr static ValueType = nullptr;

// };

// template<typename TKeym, typename T>
// struct static_map<TKey, typelist::typelist<>>
// {
//     typedef std::nullptr_t KeyType;
//     typedef std::nullptr_t ValueType;
//     constexpr static KeyType = nullptr;
//     constexpr static ValueType = nullptr;

// };


// template<typename TKey, typename ...Args>
// struct static_map<TKey, typelist::typelist<Args...>>
// {

// };

}
}