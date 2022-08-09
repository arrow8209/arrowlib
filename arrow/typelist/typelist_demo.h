#pragma once 

#include "typelist.h"

void test_typelist_type()
{
    // length find [zhuyb 2022-07-29 11:37:56]
    typedef Arrow::typelist<int, float, int, double, int> typelist_type1;
    std::cout << "typelist_type1 count:" << typelist_type1::length << std::endl;
    std::cout << "float index:" << Arrow::tlist::find<float, typelist_type1>::value << std::endl;
    std::cout << "char index:" << Arrow::tlist::find<char, typelist_type1>::value << std::endl;

    // push_back [zhuyb 2022-07-29 11:38:06]
    typedef typename Arrow::tlist::push_back<char, typelist_type1>::type typelist_type2;
    std::cout << "typelist_type2 count:" << typelist_type2::length << std::endl;
    std::cout << "char index:" << Arrow::tlist::find<char, typelist_type2>::value << std::endl;

    // pop_back [zhuyb 2022-07-29 11:38:13]
    typedef typename Arrow::tlist::pop_back<typelist_type2>::type typelist_type3;
    std::cout << "typelist_type3 count:" << typelist_type3::length << std::endl;
    std::cout << "char index:" << Arrow::tlist::find<char, typelist_type3>::value << std::endl;

    // push_front [zhuyb 2022-07-29 11:38:47]
    typedef typename Arrow::tlist::push_front<char, typelist_type3>::type typelist_type4;
    std::cout << "typelist_type4 count:" << typelist_type4::length << std::endl;
    std::cout << "char index:" << Arrow::tlist::find<char, typelist_type4>::value << std::endl;

    // pop_front [zhuyb 2022-07-29 11:38:47]
    typedef typename Arrow::tlist::pop_front<typelist_type4>::type typelist_type5;
    std::cout << "typelist_type5 count:" << typelist_type5::length << std::endl;
    std::cout << "char index:" << Arrow::tlist::find<char, typelist_type5>::value << std::endl;

    // remove [zhuyb 2022-07-29 11:38:47]
    typedef typename Arrow::tlist::remove_at<1, typelist_type5>::type typelist_type6;
    std::cout << "typelist_type5 count:" << typelist_type6::length << std::endl;
    std::cout << "int index:" << Arrow::tlist::find<int, typelist_type6>::value << std::endl;

    Arrow::tlist::print(typelist_type6{});

    typedef typename Arrow::tlist::find_last<int, typelist_type6> typelist_type7;
    std::cout << "int index " << typelist_type7::value << std::endl;
}
constexpr static char sz[] = "asdfadf";
void test_typelist_vlaue()
{
    typedef Arrow::value_typelist<6, 7, 8, 9> typelist_value1;

    std::cout << Arrow::tlist::find<Arrow::value_type<5>, typelist_value1>::value << std::endl;

    std::cout << Arrow::tlist::find<Arrow::value_type<6>, typelist_value1>::value << std::endl;

    Arrow::tlist::print(typelist_value1{});
}

void test_static_string()
{
    char szTmp[] = {MakeCharSequence_16(0 ,0, "1234")};
    std::cout << szTmp << std::endl;

    typedef STATIC_STRING("1234") static_str1;
    std::cout << "static_str1 Length:" << static_str1::length << std::endl;    
    Arrow::tlist::print(static_str1{});

}

constexpr static char data[10] = "123456789";
void test_static_string2()
{
    typedef STATIC_STRING(__FILE__) static_str_fullname;
    std::cout << "static_str1 Length:" << static_str_fullname::length << std::endl;
    Arrow::tlist::print(static_str_fullname{});
    typedef typename Arrow::tlist::tvaluelist_to_data<static_str_fullname> static_fullname;
    std::cout << static_fullname::data << std::endl;

    typedef Arrow::tvalue_typelist<int, 1, 2, 3, 4, 5> static_int_array;
    typedef Arrow::tvalue_typelist<int, 1, 2, 3, 4> static_int_array2;
    Arrow::tlist::print(static_int_array{});
    std::cout << Arrow::tlist::tvaluelist_to_data<static_int_array>::data[0];
    std::cout << Arrow::tlist::tvaluelist_to_data<static_int_array>::data[1];
    std::cout << Arrow::tlist::tvaluelist_to_data<static_int_array>::data[2];
    std::cout << Arrow::tlist::tvaluelist_to_data<static_int_array>::data[3];
    std::cout << Arrow::tlist::tvaluelist_to_data<static_int_array>::data[4] << std::endl;

    // typedef Arrow::static_string::getfilename<static_str_fullname>::type static_str_fillname;
    // std::cout << Arrow::typelist::tvaluelist_to_data<static_str_fillname>::data << std::endl;

    // // 获取文件名 [zhuyb 2022-08-03 09:06:23]
    // typedef STATIC_FILE static_filename;
    // std::cout << Arrow::typelist::tvaluelist_to_data<static_str_fillname>::data << std::endl;
}

void TestPair()
{

    typedef  Arrow::static_map<
     Arrow::static_pair<Arrow::value_type<1>,Arrow::value_type<2>>,
     Arrow::static_pair<Arrow::value_type<3>,Arrow::value_type<3>>,
     Arrow::static_pair<Arrow::value_type<5>,Arrow::value_type<4>>,
     Arrow::static_pair<Arrow::value_type<7>,Arrow::value_type<5>>
     > SMap1;
     Arrow::tlist::print(SMap1{});
     Arrow::tlist::print(typename Arrow::smap::get<Arrow::value_type<6>, SMap1>::Pair{});
     std::cout << typeid(SMap1::Pair).name() << std::endl;;
    // std::cout << s_pair1::Key << ":" << s_pair1::Value << std::endl;
}