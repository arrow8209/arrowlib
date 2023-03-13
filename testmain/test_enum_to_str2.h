#pragma once
#include <vector>
#include <string>
#include "../arrow/arrow.h"

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
    // typedef Arrow::tvalue_type<EnumTest2, _emT1> key;
    typedef typename Arrow::enum_to_str<EnumTest2,  _emT1, _emT3, _emT5, _emT9> EnumTest2Str;
    auto mapValue = EnumTest2Str::ItemInfo();
    std::cout << EnumTest2Str::ItemStr(_emT1) << std::endl;
    // typedef typename Arrow::smap::get<key, EnumTest2Str>::Pair pair;
    // std::cout << pair::value << std::endl;

    // std::cout << typeid(EnumTest2Str).name() << std::endl;
    // typedef Arrow::static_string::details::EnumItemInfo<EnumTest2, _emT8> OneItemTest;
    // std::cout << Arrow::tlist::tvaluelist_to_data<typename OneItemTest::type_full_name>::data << std::endl;
    // std::cout << Arrow::tlist::tvaluelist_to_data<typename OneItemTest::type_long_name>::data << std::endl;
    // std::cout << Arrow::tlist::tvaluelist_to_data<typename OneItemTest::type_short_name>::data << std::endl;

    // typedef Arrow::static_string::details::get_enum_str_impl<Arrow::typelist<>, EnumTest2, _emT1, _emT1>::type TypeEnumListItem;
}
