/*
 * @FilePath: /arrowlib/arrow/typelist/enum_to_str.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2023-01-29 17:50:57
 * @Description: 使用C++11实现的枚举值，最后的数据结构还是属于运行期代码。如果使用C++14或者更高版本可以改写为编译器获取参数
 */
#pragma once
#include "static_string.h"

namespace Arrow
{
namespace static_string
{

// typedef std::map<int, std::string> MapEnumValueToStr;

namespace details
{
template<int index, typename StaticStr>
struct get_enum_item_str_splite;

template<typename ...Args>
struct get_enum_item_str_splite<-1, typelist<Args...>>
{
    typedef typelist<Args...> type;
};

template<int index, typename ...Args>
struct get_enum_item_str_splite<index, typelist<Args...> >
{
protected:
    static_assert(index >= 0, "index 小于0(请检查代码逻辑)");
    typedef typename tlist::splite<index + 1, typelist<Args...>> split_str;

public:
    typedef typename split_str::Tail type;
};

template<typename StaticStr>
struct get_enum_item_str;

template<typename ... Args>
struct get_enum_item_str<typelist<Args...>>
{
protected:
    typedef typename tlist::pop_back<typelist<Args...>>::type static_str;
    typedef typename tlist::find_last<tvalue_type<char, ' '>, static_str> find_last_forward_sapce;
    typedef typename get_enum_item_str_splite<find_last_forward_sapce::value, static_str>::type enum_long_name;

    typedef typename tlist::find_last<tvalue_type<char, ':'>, enum_long_name> find_last_forward_colon;
    typedef typename get_enum_item_str_splite<find_last_forward_colon::value, enum_long_name>::type enum_short_name;

public:
    typedef enum_long_name type_long_name;
    typedef enum_short_name type_short_name;
};

template<typename TEnumType, TEnumType _emValue>
struct EnumItemInfo
{
    constexpr EnumItemInfo() : 
        short_name(tlist::tvaluelist_to_data<typename get_enum_item_str<typename STATIC_STRING(__PRETTY_FUNCTION__)>::type_short_name>::data), 
        long_name(tlist::tvaluelist_to_data<typename get_enum_item_str<typename STATIC_STRING(__PRETTY_FUNCTION__)>::type_long_name>::data), 
        value(_emValue){};
        
    const char* short_name;
    const char* long_name;
    const TEnumType value;
};

// 遵循左闭右闭 [zhuyb 2023-01-30 08:50:00]
template<typename EnumType, EnumType _emIndex, EnumType _emEnd>
struct get_enum_str
{
    template<typename TMapEnumValueToStr>
    static void ItemInfo(TMapEnumValueToStr& mapEnumValueToStr)
    {
        static_assert(_emIndex < _emEnd, "起始枚举值必须小于终止枚举值");
        EnumItemInfo<EnumType, _emIndex> tmp;
        mapEnumValueToStr[tmp.value] = tmp.short_name;
        get_enum_str<EnumType, static_cast<EnumType>(static_cast<int32_t>(_emIndex) + 1), _emEnd>::ItemInfo(mapEnumValueToStr);
    }
};

template<typename EnumType, EnumType _emEnd>
struct get_enum_str<EnumType, _emEnd, _emEnd>
{
    template<typename TMapEnumValueToStr>
    static void ItemInfo(TMapEnumValueToStr& mapEnumValueToStr)
    {
        EnumItemInfo<EnumType, _emEnd> tmp;
        mapEnumValueToStr[tmp.value] = tmp.short_name;
    }
};

}

template<typename EnumType, EnumType _emFirst,  EnumType _emLast,bool bFirstAdd = true, bool bLastAdd = true>
struct get_enum_str;


template<typename EnumType, EnumType _emFirst, EnumType _emLast>
struct get_enum_str<EnumType, _emFirst, _emLast, true, true>
{
    typedef std::map<EnumType, std::string> MapEnumValueToStr;
    static MapEnumValueToStr& ItemInfo()
    {
        static_assert(_emFirst < _emLast, "起始枚举值必须小于终止枚举值");

        static MapEnumValueToStr mapEnumValueToStr;
        if(mapEnumValueToStr.size() > 0)
            return mapEnumValueToStr;

        details::get_enum_str<EnumType, _emFirst, _emLast>::ItemInfo(mapEnumValueToStr);
        return mapEnumValueToStr;
    }

    static std::string ItemStr(EnumType _emType)
    {
        MapEnumValueToStr& mapEnumValueToStr = ItemInfo();
        auto it = mapEnumValueToStr.find(_emType);
        if(it == mapEnumValueToStr.end())
        {
            return "";
        }
        return it->second;
    }
};

template<typename EnumType, EnumType _emFirst, EnumType _emLast>
struct get_enum_str<EnumType, _emFirst, _emLast, false, true>
{
private:
    typedef get_enum_str<EnumType, static_cast<EnumType>(static_cast<int32_t>(_emFirst)+1), _emLast, true, true> get_enum_str_tt;

public:
    typedef std::map<EnumType, std::string> MapEnumValueToStr;
    static MapEnumValueToStr& ItemInfo()
    {
        return get_enum_str_tt::ItemInfo();
        // static_assert(_emFirst < _emLast, "起始枚举值必须小于终止枚举值");
        // MapEnumValueToStr& mapEnumValueToStr;

        // if(mapEnumValueToStr.size() > 0)
        //     return mapEnumValueToStr;

        // details::get_enum_str<EnumType, static_cast<EnumType>(_emFirst + 1), _emLast>::ItemInfo(mapEnumValueToStr);
        // return mapEnumValueToStr;
    }

    static std::string ItemStr(EnumType _emType)
    {
        return get_enum_str_tt::ItemStr(_emType);
        // MapEnumValueToStr& mapEnumValueToStr = ItemInfo();
        // auto it = mapEnumValueToStr.find(_emType);
        // if(it == mapEnumValueToStr.end())
        // {
        //     return "";
        // }
        // return it->second;
    }
};

template<typename EnumType, EnumType _emFirst, EnumType _emLast>
struct get_enum_str<EnumType, _emFirst, _emLast, true, false>
{
private:
    typedef get_enum_str<EnumType, _emFirst, static_cast<EnumType>(_emLast - 1), true, true> get_enum_str_tt;

public:
    typedef std::map<EnumType, std::string> MapEnumValueToStr;
    static MapEnumValueToStr& ItemInfo()
    {
        return get_enum_str_tt::ItemInfo();
        // static_assert(_emFirst < _emLast, "起始枚举值必须小于终止枚举值");
        // MapEnumValueToStr mapEnumValueToStr;

        // if(mapEnumValueToStr.size() > 0)
        //     return mapEnumValueToStr;

        // details::get_enum_str<EnumType, _emFirst, static_cast<EnumType>(_emLast - 1)>::ItemInfo(mapEnumValueToStr);
        // return mapEnumValueToStr;
    }

    static std::string ItemStr(EnumType _emType)
    {
        return get_enum_str_tt::ItemStr(_emType);
        // MapEnumValueToStr& mapEnumValueToStr = ItemInfo();
        // auto it = mapEnumValueToStr.find(_emType);
        // if(it == mapEnumValueToStr.end())
        // {
        //     return "";
        // }
        // return it->second;
    }
};

template<typename EnumType, EnumType _emFirst, EnumType _emLast>
struct get_enum_str<EnumType, _emFirst, _emLast, false, false>
{
 private:
    typedef get_enum_str<EnumType, static_cast<EnumType>(static_cast<int32_t>(_emFirst) + 1), static_cast<EnumType>(static_cast<int32_t>(_emLast) - 1), true, true> get_enum_str_tt;

public:
    typedef std::map<EnumType, std::string> MapEnumValueToStr;
    static MapEnumValueToStr& ItemInfo()
    {
        return get_enum_str_tt::ItemInfo();
        // static_assert(_emFirst < _emLast, "起始枚举值必须小于终止枚举值");
        // MapEnumValueToStr mapEnumValueToStr;
        // details::get_enum_str<EnumType, static_cast<EnumType>(_emFirst + 1), static_cast<EnumType>(_emLast - 1)>::ItemInfo(mapEnumValueToStr);
        // return mapEnumValueToStr;
    }

    static std::string ItemStr(EnumType _emType)
    {
        return get_enum_str_tt::ItemStr(_emType);
        // MapEnumValueToStr& mapEnumValueToStr = ItemInfo();
        // auto it = mapEnumValueToStr.find(_emType);
        // if(it == mapEnumValueToStr.end())
        // {
        //     return "";
        // }
        // return it->second;
    }
};

}

template<typename EnumType, EnumType _emFirst,  EnumType _emLast,bool bFirstAdd, bool bLastAdd>
using enum_to_str = static_string::get_enum_str<EnumType, _emFirst, _emLast, bFirstAdd, bLastAdd>;

}