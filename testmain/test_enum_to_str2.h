#pragma once
#include <vector>
#include <string>
#include <functional>
#include <type_traits>
#include "../arrow/arrow.h"

// template<typename TEnumType, TEnumType _dataType>
// auto GetEnumFullName() -> decltype(STATIC_STRING(__PRETTY_FUNCTION__){})
// {
//     typedef typename STATIC_STRING(__PRETTY_FUNCTION__) EnumFullStr;
//     return EnumFullStr();
// }

typedef enum _emEnumTest2
{
    _emT1 = 0,
    _emT2,
    _emT3,
    _emT4,
    _emT5,
    _emT6,
    _emT7,
    _emT8,
    _emT9
}EnumTest2;

static void TestEnum2()
{

    // typedef typename std::reference_wrapper<decltype(&GetEnumFullName<EnumTest2, _emT1>)>::result_type type_full_name;

    // typedef Arrow::ValueType<EnumTest2, _emT1> key;
    // typedef typename Arrow::enum_to_str<EnumTest2,  _emT1, _emT3, _emT5, _emT9> EnumTest2Str;
    // auto mapValue = EnumTest2Str::ItemInfo();
    // std::cout << EnumTest2Str::ItemStr(_emT1) << std::endl;
    // typedef typename Arrow::smap::get<key, EnumTest2Str>::Pair pair;
    // std::cout << pair::value << std::endl;

    // std::cout << typeid(EnumTest2Str).name() << std::endl;
    // typedef Arrow::static_string::details::EnumItemInfo<EnumTest2, _emT8> OneItemTest;
    // std::cout << Arrow::ValueListToData<typename OneItemTest::type_full_name>::data << std::endl;
    // std::cout << Arrow::ValueListToData<typename OneItemTest::type_long_name>::data << std::endl;
    // std::cout << Arrow::ValueListToData<typename OneItemTest::type_short_name>::data << std::endl;

    // typedef Arrow::static_string::details::get_enum_str_impl<Arrow::TypeList<>, EnumTest2, _emT1, _emT1>::type TypeEnumListItem;
}
