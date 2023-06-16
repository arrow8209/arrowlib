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



template<typename TEnumType, TEnumType _emValue>
struct EnumItemInfoTest
{
    // typedef Arrow::ValueListToData<typename STATIC_STRING(__FUNCTION__)> type;
    // typedef decltype([](){return STATIC_STRING(__FUNCTION__)();}) type;
    static char* full_name;

};

// template<typename TEnumType, TEnumType _emValue>
// auto FunTets() -> decltype([](){return STATIC_STRING(__FUNCTION__)();})
// {
//     // typedef Arrow::ValueListToData<typename STATIC_STRING(__FUNCTION__)> type;
//     // typedef decltype([](){return STATIC_STRING(__FUNCTION__)();}) type;
//     return STATIC_STRING(__FUNCTION__)();
// };

template<typename TEnumType, TEnumType _emValue>
char* EnumItemInfoTest<TEnumType, _emValue>::full_name = Arrow::ValueListToData<typename STATIC_STRING(__FUNCTION__)>::data;


typedef EnumItemInfoTest<EmDatType, _emDataTypeInt32> EnumItemInfoTest_Int32;
}

typedef enum _emDataTypeA
{
    _emDataTypeA1,
    _emDataTypeA2,
    _emDataTypeA3
}EmDatTypeA;
typedef Arrow::enum_to_str<EmDatTypeA, _emDataTypeA1, _emDataTypeA3> EmDatTypeAStr;

typedef enum class _emDataTypeB
{
    _emDataTypeB1 = 0,
    _emDataTypeB2 = 1,
    _emDataTypeB3 = 2,
    _emDataTypeB4 = 3,
    _emDataTypeB5 = 1004,
    _emDataTypeB6 = 1005,
    _emDataTypeB7 = 1006,
    _emDataTypeB8 = 1007,
    _emDataTypeB9,
    _emDataTypeB10 = 2000

}EmDataTypeB;
typedef Arrow::enum_to_str<EmDataTypeB,
         EmDataTypeB::_emDataTypeB1, EmDataTypeB::_emDataTypeB4, 
         EmDataTypeB::_emDataTypeB5, EmDataTypeB::_emDataTypeB9, 
         EmDataTypeB::_emDataTypeB10, EmDataTypeB::_emDataTypeB10> EmDataTypeBStr0;

// template<typename STAIC_STRING>
// constexpr typename Arrow::static_string::get_enum_str<STAIC_STRING>::type EnumStr()
// {
//     return Arrow::static_string::get_enum_str<STAIC_STRING>::type();
// }

// template<typename TEnumType, TEnumType _emValue>
// struct EnumItemInfo
// {
//     // constexpr EmItemStr():name(Arrow::ValueListToData<Arrow::static_string::get_enum_item_str<typename STATIC_STRING(__PRETTY_FUNCTION__) >::type>::data){};
//     constexpr EnumItemInfo() : name(Arrow::ValueListToData<typename Arrow::static_string::get_enum_item_str<typename STATIC_STRING(__PRETTY_FUNCTION__)>::type>::data), value(_emValue){};
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
    // std::string strTmp = Arrow::ValueListToData<EnumShortStr>::data;
    // vecEnumItemStr.push_back(strTmp);


    // typedef typename Arrow::static_string::getfilename<STATIC_STRING(__FILE__)>::type FILENAME;
    // return FILENAME();
    // std::cout <<Arrow::ValueListToData<EnumLongStr>::data <<std::endl;
        // std::cout <<Arrow::ValueListToData<EnumShortStr>::data <<std::endl;

    //     std::cout <<Arrow::ValueListToData<STATIC_FILE>::data <<std::endl;
}

template<typename TEnumType, TEnumType _dataType>
auto OutPut2()
{
    typedef typename STATIC_STRING(__PRETTY_FUNCTION__) EnumLongStr;
    return EnumLongStr();
    // typedef typename Arrow::static_string::get_enum_item_str<typename STATIC_STRING(__PRETTY_FUNCTION__) >::type EnumShortStr;
    // typedef typename Arrow::static_string::getfilename<STATIC_STRING(__FILE__)>::type FILENAME;
    // return FILENAME();
    // return float(0.0);
}

// template<EnumTest::EmDatType _dataType>
// constexpr auto OutPut2() -> decltype(Arrow::EnumItemInfo<EnumTest::EmDatType,  _dataType>())
// {
//     return  Arrow::EnumItemInfo<EnumTest::EmDatType,  _dataType>();
// }


void TestEnumToStr()
{
    // OutPut2<EnumTest::EmDatType::_emDataTypeNone>();
    // typedef EmItemStr<_emDataTypeNone>::STR EnumItemStr;
    // std::cout <<Arrow::ValueListToData<EnumItemStr>::data <<std::endl;
    // std::vector<std::string> vecEnumItemStr;
    // OutPut<_emDataTypeNone>(vecEnumItemStr);
    // OutPut<_emDataTypeInt32>(vecEnumItemStr);
    // OutPut<_emDataTypeFloat>(vecEnumItemStr);
    // Arrow::static_string::EnumItemInfo<EmDatType, _emDataTypeNone> tmp;
    // std::cout << tmp.name << std::endl;
    // std::cout << tmp.value << std::endl;
    
    // auto mapEnumStr = Arrow::static_string::get_enum_str<EnumTest::EmDatType, EnumTest::_emDataTypeNone, EnumTest::_emDataTypeFloat, false, true>::ItemInfo();
    // for (auto& it : mapEnumStr)
    // {
    //     std::cout << it.first << ":" << it.second << std::endl;
    // }
    
    // auto mapEnumStrA = Arrow::static_string::get_enum_str<EmDatTypeA, _emDataTypeA1, _emDataTypeA3, false, true>::ItemInfo();
    // for (auto& it : mapEnumStrA)
    // {
    //     std::cout << it.first << ":" << it.second << std::endl;
    // }
    // EmDataTypeBStr::ItemInfo();
    // std::cout << EmDataTypeBStr::ItemStr(EmDataTypeB::_emDataTypeB1) << std::endl;
    // auto mapTMp = EmDatTypeAStr::ItemInfo();
    // auto mapEnumStrB = EmDataTypeBStr::ItemInfo();

    // std::cout << EnumTest::EnumItemInfoTest_Int32::full_name << std::endl;
    // std::cout << EnumTest::EnumItemInfoTest_Int32::type::data << std::endl;
    // std::cout << Arrow::ValueListToData<EnumTest::EnumItemInfoTest_Int32::type>::data << std::endl;

    // typedef typename std::reference_wrapper<decltype(&EnumTest::FunTets)>::result_type typeRet;

    // std::cout << typeid(typeRet).name() << std::endl;
    // std::cout << Arrow::ValueListToData<typeRet>::data << std::endl;
    // std::cout << EmDataTypeBStr2::ItemStr(EmDataTypeB::_emDataTypeB1) << std::endl;
    // std::cout << EmDataTypeBStr1::ItemStr(EmDataTypeB::_emDataTypeB1) << std::endl;
    std::cout << EmDataTypeBStr0::ItemStr(static_cast<EmDataTypeB>(2000)) << std::endl;

    std::cout << EmDataTypeBStr0::ItemStr(EmDataTypeB::_emDataTypeB1) << std::endl;
    std::cout << EmDataTypeBStr0::ItemStr(EmDataTypeB::_emDataTypeB9) << std::endl;
    std::cout << EmDataTypeBStr0::ItemStr(EmDataTypeB::_emDataTypeB10) << std::endl;

    typedef std::reference_wrapper<decltype(&OutPut2<EnumTest::EmDatType, EnumTest::EmDatType::_emDataTypeInt32>)>::result_type typeFunName;
    std::cout << Arrow::ValueListToData<typeFunName>::data << std::endl;
}