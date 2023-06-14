/*
 * @FilePath: /arrowlib/arrow/TypeList/enum_to_str_14.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2023-01-29 17:50:57
 * @Description: 使用C++11实现的枚举值，最后的数据结构还是属于运行期代码。如果使用C++14或者更高版本可以改写为编译器获取参数
 */
#pragma once
#include <functional>
#include <type_traits>
#include "static_map.h"
#include "static_string.h"

namespace Arrow
{
namespace static_string
{

// // 枚举字符串映射表 [zhuyb 2023-03-10 10:13:52]
// template <typename TEnumType>
// struct EnumItemData
// {
//     typedef std::map<TEnumType, std::string> type;
//     static type value;
// };
// template <typename TEnumType>
// typename EnumItemData<TEnumType>::type
//     EnumItemData<TEnumType>::value;

namespace details
{
/*
g++: 编译器的函数名为 
constexpr Arrow::static_string::details::EnumItemInfo<TEnumType, _emValue>::EnumItemInfo() [with TEnumType = _emDataTypeB; TEnumType _emValue = _emDataTypeB::_emDataTypeB1]
或者（枚举类型存在，但是具体的枚举值不在）
Arrow::static_string::details::EnumItemInfo<TEnumType, _emValue>::EnumItemInfo() [with TEnumType = _emDataTypeB; TEnumType _emValue = (_emDataTypeB)2]


auto OutPut2() [with TEnumType = EnumTest::_emDataType; TEnumType _dataType = EnumTest::_emDataTypeInt32]
*/

template<typename TEnumType, TEnumType _dataType>
auto GetEnumFullName()
{
    typedef typename STATIC_STRING(__PRETTY_FUNCTION__) EnumFullStr;
    return EnumFullStr();
}

// 截取需要的字符串 [zhuyb 2023-03-10 13:28:17]
template<int index, typename StaticStr>
struct get_enum_item_str_splite;

template<typename ...Args>
struct get_enum_item_str_splite<-1, TypeList<Args...>>
{
    typedef TypeList<Args...> type;
};

template<int index, typename ...Args>
struct get_enum_item_str_splite<index, TypeList<Args...> >
{
protected:
    static_assert(index >= 0, "index 小于0(请检查代码逻辑)");
    typedef typename tlist::splite<index + 1, TypeList<Args...>> split_str;

public:
    typedef typename split_str::Tail type;
};

template<typename StaticStr>
struct get_enum_item_str;

template<typename ... Args>
struct get_enum_item_str<TypeList<Args...>>
{
protected:
    typedef typename tlist::pop_back<TypeList<Args...>>::type static_str;

    typedef typename tlist::FindLast<ValueType<char, ' '>, static_str> find_last_forward_sapce;
    typedef typename get_enum_item_str_splite<find_last_forward_sapce::value, static_str>::type enum_long_name;

    typedef typename tlist::FindLast<ValueType<char, ':'>, enum_long_name> find_last_forward_colon;
    typedef typename get_enum_item_str_splite<find_last_forward_colon::value, enum_long_name>::type enum_short_name;

public:
    typedef enum_long_name type_long_name;
    typedef enum_short_name type_short_name;
};


template<typename TEnumType, TEnumType _emValue>
struct EnumItemInfo
{
    typedef typename std::reference_wrapper<decltype(&GetEnumFullName<TEnumType, _emValue>)>::result_type type_full_name;
    typedef typename get_enum_item_str<type_full_name>::type_long_name type_long_name;
    typedef typename get_enum_item_str<type_full_name>::type_short_name type_short_name;
    constexpr static TEnumType value = _emValue;
};

// // template<typename MapItemData, typename EnumType, EnumType _emFirst,  EnumType _emLast>
// // struct get_enum_str_impl;

// template<typename MapItemData, typename EnumType, EnumType _emFirst,  EnumType _emLast>
// struct get_enum_str_impl
// {
// protected:
//     typedef ValueType<EnumType, _emFirst> key;
//     typedef typename EnumItemInfo<EnumType, _emFirst>::type_short_name value;
//     typedef static_pair<key, value> item;   
//     typedef typename Arrow::smap::insert<MapItemData, item>::type mapItemData;

// public:
//     typedef typename get_enum_str_impl<mapItemData, EnumType, static_cast<EnumType>(static_cast<int32_t>(_emFirst) + 1), _emLast>::type type;
// };

// template<typename MapItemData, typename EnumType, EnumType _emItem>
// struct get_enum_str_impl<MapItemData, EnumType, _emItem, _emItem>
// {
// protected:
//     typedef ValueType<EnumType, _emItem> key;
//     typedef typename EnumItemInfo<EnumType, _emItem>::type_short_name value;
//     typedef static_pair<key, value> item;   
// public:
//     typedef typename Arrow::smap::insert<MapItemData, item>::type type;
// };


template<typename MapItemData, typename EnumType, EnumType ... enumTypes>
struct get_enum_str_impl;

template<typename MapItemData, typename EnumType, EnumType _emItem>
struct get_enum_str_impl<MapItemData, EnumType, _emItem, _emItem>
{
protected:
    typedef ValueType<EnumType, _emItem> key;
    typedef typename EnumItemInfo<EnumType, _emItem>::type_short_name value;
    typedef static_pair<key, value> item;   
public:
    typedef typename Arrow::smap::insert<MapItemData, item>::type type;
};

template<typename MapItemData, typename EnumType, EnumType _emFirst,  EnumType _emLast>
struct get_enum_str_impl<MapItemData, EnumType, _emFirst, _emLast>
{
protected:
    typedef ValueType<EnumType, _emFirst> key;
    typedef typename EnumItemInfo<EnumType, _emFirst>::type_short_name value;
    typedef static_pair<key, value> item;   
    typedef typename Arrow::smap::insert<MapItemData, item>::type mapItemData;

public:
    typedef typename get_enum_str_impl<mapItemData, EnumType, static_cast<EnumType>(static_cast<int32_t>(_emFirst) + 1), _emLast>::type type;
};

template<typename MapItemData, typename EnumType, EnumType _emFirst,  EnumType _emLast, EnumType ...enumTypes>
struct get_enum_str_impl<MapItemData, EnumType, _emFirst, _emLast, enumTypes...>
{
protected:
    typedef typename get_enum_str_impl<MapItemData, EnumType, _emFirst, _emLast>::type mapItemData;

public:
    typedef typename get_enum_str_impl<mapItemData,EnumType, enumTypes...>::type type;
};

}

template<typename EnumType, EnumType ... enumTypes>
struct get_enum_str
{
    typedef typename details::get_enum_str_impl<static_map<>, EnumType, enumTypes...>::type type;

    static auto& ItemInfo()
    {
        return smap::smap_to_map<type>::Data();
    }


    static const char* ItemStr(EnumType emType)
    {
        auto& tmpMap = smap::smap_to_map<type>::Data();
        auto it = tmpMap.find(emType);
        if(it == tmpMap.end())
            return "";
        return it->second;
    }
};

}

template<typename EnumType, EnumType ...enumTypes>
using enum_to_str=static_string::get_enum_str<EnumType, enumTypes...>;


}