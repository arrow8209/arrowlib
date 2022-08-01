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

    Arrow::typelist::print(typelist_type6{});
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
    // Arrow::typelist::printf_valuetype_list<static_str1>::print();
    Arrow::typelist::print(static_str1{});

    // typedef Arrow::typelist::splite<sizeof("1234") - 1, static_str1>::Head static_str2;
    // std::cout << "static_str2 Length:" << static_str2::length << std::endl;
    // Arrow::typelist::printf_valuetype_list<static_str2>::print();

    // typedef Arrow::typelist::concat<static_str2, static_str2>::type static_str3;
    // std::cout << Arrow::typelist::tvaluelist_to_data<static_str3>::data << std::endl;
    // #define CHARLIST '1', '2', 0, 0

    // typedef decltype(Arrow::typelist::CheckStr(Arrow::typelist::tvalue_type<char, '1'>{},
    //                                            Arrow::typelist::tvalue_type<char, '2'>{},
    //                                            Arrow::typelist::tvalue_type<char, 0>{},
    //                                            Arrow::typelist::tvalue_type<char, 0>{})) CheckStr1_A;
    // Arrow::typelist::printf_valuetype_list<CheckStr1_A>::print();
    // const char sz = Arrow::static_string::get<0>("123");
    // CheckStr1_A::InfoPrint();
    // typedef typename Arrow::typelist::CheckStr<Arrow::typelist::statc_string_t<char, '1', '2'>,
    //                                            Arrow::typelist::statc_string_t<char, 0>,
    //                                            Arrow::typelist::statc_string_t<char, 0>>::type static_str2;

    // typedef Arrow::typelist::tvalue_typelist<char,
    //                                          Arrow::static_string::get<0>("123456"),
    //                                          Arrow::static_string::get<2>("123456"),
    //                                          Arrow::static_string::get<3>("123456"),
    //                                          Arrow::static_string::get<4>("123456"),
    //                                          Arrow::static_string::get<6>("123456"),
    //                                          Arrow::static_string::get<8>("123456")>
    //     sz_typuelist;
    // typedef STATIC_STRING("1") static_str;
    // typedef Arrow::typelist::typelist<Arrow::typelist::tvalue_type<char, '1'>,
    //                                          Arrow::typelist::tvalue_type<char, '2'>,
    //                                          Arrow::typelist::tvalue_type<char, '3'>,
    //                                          Arrow::typelist::tvalue_type<char, '4'>,
    //                                          Arrow::typelist::tvalue_type<char, 0>,
    //                                          Arrow::typelist::tvalue_type<char, 0>,
    //                                          Arrow::typelist::tvalue_type<char, 0>>
    //     sz_typuelist;


    // typedef typename Arrow::typelist::static_string<sz_typuelist>::type static_str;

    // std::cout <<Arrow::typelist::length<static_str>::value << std::endl;
    // std::cout << "static_str Lenght:" << Arrow::typelist::length<static_str>::value << std::endl;
    // std::cout << "static_str tvalue_char<char, 0> index:" << Arrow::typelist::find<Arrow::typelist::tvalue_type<char, 0>, static_str>::value << std::endl;
    // Arrow::typelist::printf_valuetype_list<sz_typuelist>::print();

    // typedef Arrow::tvalue_typelist<char, '6', '7', '8', '9'> typelist_value_t1;

    // std::cout << Arrow::typelist::index_of<Arrow::tvalue_type<char, '5'>, typelist_value_t1>::value << std::endl;
    // std::cout << Arrow::typelist::index_of<Arrow::tvalue_type<char, '8'>, typelist_value_t1>::value << std::endl;


    // typedef Arrow::typelist::typelist<float, int> typelist_t1;
    // typedef Arrow::typelist::remove_int<typelist_t1>::type typuelist_t2;

    // Arrow::typelist::printf_typelist<typelist_t1>::print();
    // Arrow::typelist::printf_typelist<typuelist_t2>::print();
}