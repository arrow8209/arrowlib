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


template <typename... Args>
struct static_map;

template <>
struct static_map<>
{
    typedef typelist<> type;

    template<typename TKey>
    using get=typename find<TKey, type>::Pair;
};

template<typename Key, typename Value>
struct static_map<static_pair<Key, Value>>
{
    private:
    using local=static_map<static_pair<Key, Value>>;
    public:
    using type=typelist<static_pair<Key, Value>> ;

    template<typename TKey>
    using get=typename find<TKey, local::type>::Pair;
    
};

template <typename Key, typename Value, typename... Args>
struct static_map<static_pair<Key, Value>, Args...>
{
private:
    using local=static_map<static_pair<Key, Value>, Args...>;
public:
    // 对后面的数据处理 [zhuyb 2022-08-08 23:45:25]
    typedef typename static_map<Args...>::type tail;

    // 查找后面有没有重复的key [zhuyb 2022-08-08 23:44:38]
    typedef typename find<Key, typelist<Args...>>::Pair Pair;

    // 没有重复的key [zhuyb 2022-08-08 23:44:58]
    typedef typename tlist::push_front<static_pair<Key, Value>, tail>::type one_key;

    // 有重复的key [zhuyb 2022-08-08 23:45:10]
    typedef tail two_key;

    static_assert(std::is_same<Pair, static_pair_null>::value, "static_map 存在重复选项");

    typedef typename std::conditional<std::is_same<Pair, static_pair_null>::value, one_key, two_key>::type type;

    template<typename TKey>
    using get=typename find<TKey, local::type>::Pair;
};

template<typename StaticMap>
struct smap_to_map;

template <typename Key, typename Value>
struct smap_to_map<static_map<static_pair<Key, Value>>>
{
private:
    typedef static_pair<Key, Value> Item;

public:
    template <typename MapType>
    static MapType& Data(MapType& data)
    {
        data[Item::key] = (typename Item::ValueType*)(Item::value);
        return data;
    }
};

template <typename Key, typename Value, typename... Args>
struct smap_to_map<static_map<static_pair<Key, Value>, Args...>>
{
private:
    typedef static_pair<Key, Value> Item;
    typedef std::map<typename Item::KeyType,typename Item::ValueType*> MapType;
public:
    static MapType& Data(MapType& data)
    {
        data[Item::key] = (typename Item::ValueType*)(Item::value);
        smap_to_map<static_map<Args...>>::Data(data);
        return data;
    }

    static MapType& Data()
    {
        static MapType mapData;
        if(mapData.size() > 0)
            return mapData;

        Data(mapData);
        return mapData;
    }
};


// template<typename T, typename TPairList>
// struct get;

// // 获取map中指定Key的数据 [zhuyb 2022-09-14 18:23:30]
// template<typename Key, typename ...Args>
// struct get<Key, static_map<Args...>>
// {
//     typedef typename find<Key, typename static_map<Args...>::type>::Pair Pair;
//     typedef typename find<Key, typename static_map<Args...>::type>::Tail Tail;
// };


//插入
template<typename StaticMap, typename StaticPair>
struct insert;

// template<typename Key, typename Value, typename ...Args>
// struct insert<static_map<Args...>, static_pair<Key, Value>>
// {
//     typedef static_pair<Key, Value> Pair;
//     typedef static_map<static_pair<Key, Value>, Args...> type;
// };

template<typename Key, typename Value, typename ...Args>
struct insert<static_map<Args...>, static_pair<Key, Value>>
{
public:
    // 对后面的数据处理 [zhuyb 2022-08-08 23:45:25]
    typedef typename static_map<Args...>::type tail;

    // 查找后面有没有重复的key [zhuyb 2022-08-08 23:44:38]
    typedef typename find<Key, typelist<Args...>>::Pair Pair;

    static_assert(std::is_same<Pair, static_pair_null>::value, "static_map 存在重复选项");

public:
    typedef static_map<static_pair<Key, Value>, Args...> type;
};

}

template<typename Key, typename Value>
using  static_pair = smap::static_pair<Key, Value>;

template <typename ...Args>
using static_map = smap::static_map<Args...>;

template<typename TPariList, typename Item>
using insert = smap::insert<TPariList, Item>;

namespace tlist
{


static void print(smap::static_pair_null)
{
    std::cout<< "(nullptr, nullptr)" << std::endl;
}

template <typename Key, typename Value>
static void print(static_pair<Key, Value>)
{
    std::cout<< "("<< static_pair<Key, Value>::key << "," << static_pair<Key, Value>::value << ")" << std::endl;
}

template <typename Key, typename Value, typename ...Args>
static void print(typelist<static_pair<Key, Value>, Args...>)
{
    print(static_pair<Key, Value>{});
    std::cout << "\t";
    print(typelist<Args...>{});
}

template <typename... Args>
static void print(static_map<Args...>)
{
    print(typename static_map<Args...>::type{});
}

}
}