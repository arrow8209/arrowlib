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

// 枚举字符串映射表 [zhuyb 2023-03-10 10:13:52]
template <typename TEnumType>
struct EnumItemData
{
    typedef std::map<TEnumType, std::string> type;
    static type value;
};
template <typename TEnumType>
typename EnumItemData<TEnumType>::type
    EnumItemData<TEnumType>::value;

namespace details
{
/*
g++: 编译器的函数名为 
constexpr Arrow::static_string::details::EnumItemInfo<TEnumType, _emValue>::EnumItemInfo() [with TEnumType = _emDataTypeB; TEnumType _emValue = _emDataTypeB::_emDataTypeB1]
或者（枚举类型存在，但是具体的枚举值不在）
Arrow::static_string::details::EnumItemInfo<TEnumType, _emValue>::EnumItemInfo() [with TEnumType = _emDataTypeB; TEnumType _emValue = (_emDataTypeB)2]
*/

// 截取需要的字符串 [zhuyb 2023-03-10 13:28:17]
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
        full_name(tlist::tvaluelist_to_data<typename STATIC_STRING(__PRETTY_FUNCTION__)>::data),
        short_name(tlist::tvaluelist_to_data<typename get_enum_item_str<typename STATIC_STRING(__PRETTY_FUNCTION__)>::type_short_name>::data), 
        long_name(tlist::tvaluelist_to_data<typename get_enum_item_str<typename STATIC_STRING(__PRETTY_FUNCTION__)>::type_long_name>::data), 
        value(_emValue){};
        
    const char* short_name;
    const char* long_name;
    const char* full_name;
    const TEnumType value;
};

template<typename EnumType, EnumType _emIndex, EnumType _emEnd>
struct get_enum_str_impl
{
    static void ItemInfo()
    {
        static_assert(_emIndex < _emEnd, "起始枚举值必须小于终止枚举值");
        EnumItemInfo<EnumType, _emIndex> tmp;
        EnumItemData<EnumType>::value[tmp.value] = tmp.short_name;
        get_enum_str_impl<EnumType, static_cast<EnumType>(static_cast<int32_t>(_emIndex) + 1), _emEnd>::ItemInfo();
    }
};

template<typename EnumType, EnumType _emEnd>
struct get_enum_str_impl<EnumType, _emEnd, _emEnd>
{
    static void ItemInfo()
    {
        EnumItemInfo<EnumType, _emEnd> tmp;
        EnumItemData<EnumType>::value[tmp.value] = tmp.short_name;
    }
};

}

template<typename EnumType, EnumType _emFirst,  EnumType _emLast>
struct get_enum_str
{

    static typename EnumItemData<EnumType>::type& ItemInfo()
    {
        static_assert(_emFirst <= _emLast, "起始枚举值必须小于终止枚举值");

        // if(EnumItemData<EnumType>::value.size() > 0)
        //     return EnumItemData<EnumType>::value;

        details::get_enum_str_impl<EnumType, _emFirst, _emLast>::ItemInfo();
        return EnumItemData<EnumType>::value;
    }

    static std::string ItemStr(EnumType _emType)
    {
        if(_emType > _emLast || _emType < _emFirst)
        {
            return "";
        }

        auto it = EnumItemData<EnumType>::value.find(_emType);
        if(it == EnumItemData<EnumType>::value.end())
        {
            ItemInfo();
        }

        it = EnumItemData<EnumType>::value.find(_emType);
        if(it == EnumItemData<EnumType>::value.end())
        {
            return "";
        }

        return it->second;
    }
};

template<typename EnumType, EnumType ... enumTypes>
struct get_enum_str2;

template<typename EnumType>
struct get_enum_str2<EnumType>
{
    static typename EnumItemData<EnumType>::type& ItemInfo()
    {
        return EnumItemData<EnumType>::value;
    }

    static std::string ItemStr(EnumType _emType)
    {
        return "";
    }
};

template<typename EnumType, EnumType _emFirst,  EnumType _emLast>
struct get_enum_str2<EnumType, _emFirst, _emLast>
{
private:
    typedef get_enum_str<EnumType, _emFirst, _emLast> tmp_get_enum_str;
public:
    static typename EnumItemData<EnumType>::type& ItemInfo()
    {
        return tmp_get_enum_str::ItemInfo();
    }

    static std::string ItemStr(EnumType _emType)
    {
        return tmp_get_enum_str::ItemStr(_emType);
    }
};

template<typename EnumType, EnumType _emFirst,  EnumType _emLast, EnumType ...enumTypes>
struct get_enum_str2<EnumType, _emFirst, _emLast, enumTypes...>
{
private:
    typedef get_enum_str<EnumType, _emFirst, _emLast> tmp_get_enum_str1;
    typedef get_enum_str2<EnumType, enumTypes...> tmp_get_enum_str2;
public:
    static typename EnumItemData<EnumType>::type& ItemInfo()
    {
        return tmp_get_enum_str2::ItemInfo();
    }

    static std::string ItemStr(EnumType _emType)
    {
        if(_emType >= _emFirst && _emType <= _emLast)
        {
            return tmp_get_enum_str1::ItemStr(_emType);
        }

        return tmp_get_enum_str2::ItemStr(_emType);
    }
};

}

template<typename EnumType, EnumType ...enumTypes>
using enum_to_str=static_string::get_enum_str2<EnumType, enumTypes...>;

// template<typename EnumType, EnumType emValue>
// using EnumItemInfo = static_string::details::EnumItemInfo<EnumType, emValue>;
}