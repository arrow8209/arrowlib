#pragma once 
#include "typelist.h"


void test_typelist_type()
{
    // length find [zhuyb 2022-07-29 11:37:56]
    typedef Arrow::typelist::typelist<float, int, double> typelist_type1;
    std::cout << "typelist_type1 count:" << typelist_type1::length << std::endl;
    std::cout << "float index:" << Arrow::typelist::find<float, typelist_type1>::value << std::endl;
    std::cout << "char index:" << Arrow::typelist::find<char, typelist_type1>::value << std::endl;

    // push_back [zhuyb 2022-07-29 11:38:06]
    typedef typename Arrow::typelist::push_back<char, typelist_type1>::type typelist_type2;
    std::cout << "typelist_type2 count:" << typelist_type2::length << std::endl;
    std::cout << "char index:" << Arrow::typelist::find<char, typelist_type2>::value << std::endl;

    // pop_back [zhuyb 2022-07-29 11:38:13]
    typedef typename Arrow::typelist::pop_back<typelist_type2>::type typelist_type3;
    std::cout << "typelist_type3 count:" << typelist_type3::length << std::endl;
    std::cout << "char index:" << Arrow::typelist::find<char, typelist_type3>::value << std::endl;

    // push_front [zhuyb 2022-07-29 11:38:47]
    typedef typename Arrow::typelist::push_front<char, typelist_type3>::type typelist_type4;
    std::cout << "typelist_type4 count:" << typelist_type4::length << std::endl;
    std::cout << "char index:" << Arrow::typelist::find<char, typelist_type4>::value << std::endl;

    // pop_front [zhuyb 2022-07-29 11:38:47]
    typedef typename Arrow::typelist::pop_front<typelist_type4>::type typelist_type5;
    std::cout << "typelist_type5 count:" << typelist_type5::length << std::endl;
    std::cout << "char index:" << Arrow::typelist::find<char, typelist_type5>::value << std::endl;

    // remove [zhuyb 2022-07-29 11:38:47]
    typedef typename Arrow::typelist::remove_at<1, typelist_type5>::type typelist_type6;
    std::cout << "typelist_type5 count:" << typelist_type6::length << std::endl;
    std::cout << "int index:" << Arrow::typelist::find<int, typelist_type6>::value << std::endl;

    // Arrow::typelist::print(typelist_type6{});
}
constexpr static char sz[] = "asdfadf";
void test_typelist_vlaue()
{
    typedef Arrow::typelist::value_typelist<6, 7, 8, 9> typelist_value1;

    std::cout << Arrow::typelist::find<Arrow::typelist::value_type<5>, typelist_value1>::value << std::endl;

    std::cout << Arrow::typelist::find<Arrow::typelist::value_type<6>, typelist_value1>::value << std::endl;

    Arrow::typelist::print(typelist_value1{});
}

void test_static_string()
{
    char szTmp[] = {MakeCharSequence_16(0 ,0, "1234")};
    std::cout << szTmp << std::endl;

    typedef STATIC_STRING("1234") static_str1;
    std::cout << "static_str1 Length:" << static_str1::length << std::endl;    
    Arrow::typelist::print(static_str1{});

}

void test_static_string2()
{
    typedef STATIC_STRING(__FILE__) static_str_filename;
    std::cout << "static_str1 Length:" << static_str_filename::length << std::endl;
    Arrow::typelist::print(static_str_filename{});
    std::cout << Arrow::typelist::tvaluelist_to_data<static_str_filename>::data << std::endl;
}