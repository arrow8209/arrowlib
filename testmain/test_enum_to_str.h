#pragma once
#include <vector>
#include <string>
#include "arrow/typelist/typelist.h"

namespace EnumTest
{

typedef enum _emDataType
{
    _emDataTypeNone,
    _emDataTypeInt32,
    _emDataTypeFloat
}EmDatType;
}

typedef enum _emDataTypeA
{
    _emDataTypeA1,
    _emDataTypeA2,
    _emDataTypeA3
}EmDatTypeA;
// template<typename STAIC_STRING>
// constexpr typename Arrow::static_string::get_enum_str<STAIC_STRING>::type EnumStr()
// {
//     return Arrow::static_string::get_enum_str<STAIC_STRING>::type();
// }

// template<typename TEnumType, TEnumType _emValue>
// struct EnumItemInfo
// {
//     // constexpr EmItemStr():name(Arrow::tlist::tvaluelist_to_data<Arrow::static_string::get_enum_item_str<typename STATIC_STRING(__PRETTY_FUNCTION__) >::type>::data){};
//     constexpr EnumItemInfo() : name(Arrow::tlist::tvaluelist_to_data<typename Arrow::static_string::get_enum_item_str<typename STATIC_STRING(__PRETTY_FUNCTION__)>::type>::data), value(_emValue){};
//     const char* name;
//     const int value;
// };

template<EnumTest::EmDatType _dataType>
constexpr void OutPut()
{
    // return std::string_view{__PRETTY_FUNCTION__};
    // typedef typename Arrow::static_string::get_enum_str<STATIC_STRING(__PRETTY_FUNCTION__)>::type enumStrType;
    // typedef typename Arrow::static_string::get_enum_str<STATIC_STRING(__PRETTY_FUNCTION__)>::type enmuStr;
    // std::cout << enmuStr::data << std::endl;

    // typedef typename STATIC_STRING(__PRETTY_FUNCTION__) EnumLongStr;
    // typedef typename Arrow::static_string::get_enum_item_str<typename STATIC_STRING(__PRETTY_FUNCTION__) >::type EnumShortStr;
    // std::string strTmp = Arrow::tlist::tvaluelist_to_data<EnumShortStr>::data;
    // vecEnumItemStr.push_back(strTmp);


    // typedef typename Arrow::static_string::getfilename<STATIC_STRING(__FILE__)>::type FILENAME;
    // return FILENAME();
    // std::cout <<Arrow::tlist::tvaluelist_to_data<EnumLongStr>::data <<std::endl;
        // std::cout <<Arrow::tlist::tvaluelist_to_data<EnumShortStr>::data <<std::endl;

    //     std::cout <<Arrow::tlist::tvaluelist_to_data<STATIC_FILE>::data <<std::endl;
}

void TestEnumToStr()
{
    // typedef EmItemStr<_emDataTypeNone>::STR EnumItemStr;
    // std::cout <<Arrow::tlist::tvaluelist_to_data<EnumItemStr>::data <<std::endl;
    // std::vector<std::string> vecEnumItemStr;
    // OutPut<_emDataTypeNone>(vecEnumItemStr);
    // OutPut<_emDataTypeInt32>(vecEnumItemStr);
    // OutPut<_emDataTypeFloat>(vecEnumItemStr);
    // Arrow::static_string::EnumItemInfo<EmDatType, _emDataTypeNone> tmp;
    // std::cout << tmp.name << std::endl;
    // std::cout << tmp.value << std::endl;
    
    auto mapEnumStr = Arrow::static_string::get_enum_str<EnumTest::EmDatType, EnumTest::_emDataTypeNone, EnumTest::_emDataTypeFloat, false, true>::ItemInfo();
    for (auto& it : mapEnumStr)
    {
        std::cout << it.first << ":" << it.second << std::endl;
    }
    
    auto mapEnumStrA = Arrow::static_string::get_enum_str<EmDatTypeA, _emDataTypeA1, _emDataTypeA3, false, true>::ItemInfo();
    for (auto& it : mapEnumStrA)
    {
        std::cout << it.first << ":" << it.second << std::endl;
    }
}