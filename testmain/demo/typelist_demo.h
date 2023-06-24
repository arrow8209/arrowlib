#pragma once 

// #include "TypeList.h"
// #include "../other/std_assist.h"
#include "arrow/typelist/typelist.h"
#include "arrow/other/std_assist.h"
#include "arrow/typelist/static_string2.h"
#include "arrow/typelist/type_name2.h"

void test_typelist_type()
{
    // length Find [zhuyb 2022-07-29 11:37:56]
    typedef Arrow::TypeList<int, float, int, double, int> typelist_type1;
    
    std::cout << "typelist_type1 count:" << typelist_type1::length << std::endl;
    std::cout << "float index:" << Arrow::Find<float, typelist_type1>::value << std::endl;
    std::cout << "char index:" << Arrow::Find<char, typelist_type1>::value << std::endl;

    // PushBack [zhuyb 2022-07-29 11:38:06]
    typedef typename Arrow::PushBack<char, typelist_type1>::type typelist_type2;
    std::cout << "typelist_type2 count:" << typelist_type2::length << std::endl;
    std::cout << "char index:" << Arrow::Find<char, typelist_type2>::value << std::endl;

    // pop_back [zhuyb 2022-07-29 11:38:13]
    typedef typename Arrow::PopBack<typelist_type2>::type typelist_type3;
    std::cout << "typelist_type3 count:" << typelist_type3::length << std::endl;
    std::cout << "char index:" << Arrow::Find<char, typelist_type3>::value << std::endl;

    // PushFront [zhuyb 2022-07-29 11:38:47]
    typedef typename Arrow::PushFront<char, typelist_type3>::type typelist_type4;
    std::cout << "typelist_type4 count:" << typelist_type4::length << std::endl;
    std::cout << "char index:" << Arrow::Find<char, typelist_type4>::value << std::endl;

    // PopFront [zhuyb 2022-07-29 11:38:47]
    typedef typename Arrow::PopFront<typelist_type4>::type typelist_type5;
    std::cout << "typelist_type5 count:" << typelist_type5::length << std::endl;
    std::cout << "char index:" << Arrow::Find<char, typelist_type5>::value << std::endl;

    // Remove [zhuyb 2022-07-29 11:38:47]
        std::cout << typeid(typelist_type5).name() <<std::endl;
    typedef typename Arrow::Remove<1, typelist_type5>::type typelist_type6;
    std::cout << typeid(typelist_type6).name() <<std::endl;
    std::cout << "typelist_type5 count:" << typelist_type6::length << std::endl;

    std::cout << "int index:" << Arrow::Find<int, typelist_type6>::value << std::endl;


    std::cout << Arrow::TypeName<typelist_type6>::Name() << std::endl;

    typedef typename Arrow::FindLast<int, typelist_type6> typelist_type7;
    std::cout << "int index " << typelist_type7::value << std::endl;

    std::cout << "int index:" << Arrow::FindLast_v<int, typelist_type1> << std::endl;
    std::cout << "double index:" << Arrow::FindLast_v<double, typelist_type1> << std::endl;
    std::cout << "float index:" << Arrow::FindLast_v<float, typelist_type1> << std::endl;
}
constexpr static char sz[] = "asdfadf";
void test_typelist_vlaue()
{
    typedef Arrow::IntValueTypeList<6, 7, 8, 9> typelist_value1;

    std::cout << Arrow::TypeName<typelist_value1>::Name() << std::endl;
    // std::cout << Arrow::Find<Arrow::IntValueType<5>, typelist_value1>::value << std::endl;

    // std::cout << Arrow::Find<Arrow::IntValueType<6>, typelist_value1>::value << std::endl;

    // Arrow::print(typelist_value1{});
}

void test_static_string()
{
    char szTmp[] = {MakeCharSequence_16(0 ,0, "1234")};
    std::cout << szTmp << std::endl;

    typedef STATIC_STRING("1234") static_str1;
    std::cout << "static_str1 Length:" << static_str1::length << std::endl;

    std::cout << Arrow::TypeName<static_str1>::Name() << std::endl;
    // Arrow::print(static_str1{});

}

void test_static_string2()
{
    typedef STATIC_STRING(__FILE__) static_str_fullname;
    std::cout << "static_str1 Length:" << static_str_fullname::length << std::endl;

    std::cout << Arrow::TypeName<static_str_fullname>::Name() << std::endl;
    // Arrow::print(static_str_fullname{});

    typedef typename Arrow::ValueListToData<static_str_fullname> static_fullname;
    std::cout << static_fullname::data << std::endl;

    typedef Arrow::ValueTypeList<int, 1, 2, 3, 4, 5> static_int_array;
    typedef Arrow::ValueTypeList<int, 1, 2, 3, 4> static_int_array2;

    std::cout << Arrow::TypeName<static_int_array>::Name() << std::endl;
    // Arrow::print(static_int_array{});

    std::cout << Arrow::ValueListToData<static_int_array>::data[0];
    std::cout << Arrow::ValueListToData<static_int_array>::data[1];
    std::cout << Arrow::ValueListToData<static_int_array>::data[2];
    std::cout << Arrow::ValueListToData<static_int_array>::data[3];
    std::cout << Arrow::ValueListToData<static_int_array>::data[4] << std::endl;

    // typedef STATIC_FILE ssFile;
    // std::cout << Arrow::static_string::SSToDJB<ssFile>::value << std::endl;
    // std::cout << __FILE__ << std::endl;

    // char sz[] ="h.omed_tsilepyt/tsilepyt/worra/..";
    // std::cout << Arrow::Other::StrToUInt32(sz) << std::endl;


    typedef STATIC_FILE ssFile;
    std::cout << Arrow::static_string::SSToDJB<ssFile>::value << std::endl;
    std::cout << Arrow::ValueListToData<ssFile>::data << "?" << std::endl;
// typelist_demo.h
    char sz[] ="h.omed_tsilepyt";
    std::cout << Arrow::Other::StrToUInt32(sz) << std::endl;

    // typedef Arrow::static_string::getfilename<static_str_fullname>::type static_str_fillname;
    // std::cout << Arrow::TypeList::ValueListToData<static_str_fillname>::data << std::endl;

    // // 获取文件名 [zhuyb 2022-08-03 09:06:23]
    // typedef STATIC_FILE static_filename;
    // std::cout << Arrow::TypeList::ValueListToData<static_str_fillname>::data << std::endl;
}

void TestPair()
{

    typedef  Arrow::static_map<
     Arrow::static_pair<Arrow::IntValueType<1>,Arrow::IntValueType<2>>,
     Arrow::static_pair<Arrow::IntValueType<3>,Arrow::IntValueType<3>>,
     Arrow::static_pair<Arrow::IntValueType<5>,Arrow::IntValueType<4>>,
     Arrow::static_pair<Arrow::IntValueType<7>,Arrow::IntValueType<5>>
     > SMap1;

    std::cout << Arrow::TypeName<SMap1>::Name() << std::endl;
    // Arrow::print(SMap1{});

    std::cout << Arrow::TypeName<SMap1::get<Arrow::IntValueType<6>>>::Name() << std::endl;
    // Arrow::print(SMap1::get<Arrow::IntValueType<6>>{});

    std::cout << typeid(SMap1::Pair).name() << std::endl;;
    // std::cout << s_pair1::Key << ":" << s_pair1::Value << std::endl;
}



void TestPair2()
{

    typedef  Arrow::static_map<
     Arrow::static_pair<Arrow::IntValueType<1>,STATIC_STRING("1234")>,
     Arrow::static_pair<Arrow::IntValueType<3>,STATIC_STRING("2345")>,
     Arrow::static_pair<Arrow::IntValueType<5>,STATIC_STRING("3456")>,
     Arrow::static_pair<Arrow::IntValueType<7>,STATIC_STRING("4567")>
     > SMap1;

    std::cout << Arrow::TypeName<SMap1>::Name() << std::endl;
    // Arrow::print(SMap1{});

     
    std::cout << Arrow::TypeName<SMap1::get<Arrow::IntValueType<7>>>::Name() << std::endl;
    //  Arrow::print(typename Arrow::smap::get<Arrow::IntValueType<7>, SMap1>::Pair{});
    //  Arrow::print(SMap1::get<Arrow::IntValueType<7>>{});
    
     using it=SMap1::get<Arrow::IntValueType<7>>;
    std::cout << typeid(it).name() << std::endl;
     std::cout << typeid(SMap1::Pair).name() << std::endl;
    // std::cout << s_pair1::Key << ":" << s_pair1::Value << std::endl;
}

static void TestIntSquence()
{
    // constexpr int n1 = Arrow::StaticStr::Find(__PRETTY_FUNCTION__, 'I');
    // using IntSequece5 = Arrow::MakeIntegerSequence<n1>::type;
    // std::cout << Arrow::TypeName<IntSequece5>::Name() << std::endl;
    
}

static void TestTypeName()
{
    typedef Arrow::TypeList<> typelist_type1;
    std::cout << Arrow::TypeName<typelist_type1>::Name() << std::endl;
}

static void TestStaticSubStr()
{
    // constexpr auto str = __PRETTY_FUNCTION__;
    // constexpr int n1 = Arrow::StaticStr::Find(__PRETTY_FUNCTION__, 'e');
    // constexpr auto s =Arrow::StaticStr::SubStr<0, n1>(str);
    // constexpr auto s0 = Arrow::StaticStr::details::SubStrImpl<Arrow::MakeIntegerSequence<n1>::type>(str);
    // constexpr auto s1 = Arrow::StaticStr::details::SubStrImpl<Arrow::MakeIntegerSequence<n1>::type>(str, 5);

    // using IntSequece5 = Arrow::MakeIntegerSequence<n1>::type;

    // auto name = Arrow::TypeName2<IntSequece5>::Name();

    // std::cout << Arrow::TypeName2<std::vector<int>>::Name() << std::endl;

    // std::cout << Arrow::TypeNameFun(IntSequece5{}) << std::endl;

    // auto s2 = Arrow::TypeNameFun(IntSequece5{});

    // constexpr auto s3 = Arrow::StringView<sizeof(__PRETTY_FUNCTION__)>(__PRETTY_FUNCTION__);

    //     typedef  Arrow::static_map<
    //  Arrow::static_pair<Arrow::IntValueType<1>,STATIC_STRING("1234")>,
    //  Arrow::static_pair<Arrow::IntValueType<3>,STATIC_STRING("2345")>,
    //  Arrow::static_pair<Arrow::IntValueType<5>,STATIC_STRING("3456")>,
    //  Arrow::static_pair<Arrow::IntValueType<7>,STATIC_STRING("4567")>
    //  > SMap1;

    // std::cout << Arrow::TypeName2<SMap1>::Name() << std::endl;
}

static void TestStringView()
{
    // constexpr auto st1 = Arrow::StaticStr::Str("123");
    // constexpr auto st2 = Arrow::StaticStr::Str("4567");
    // constexpr auto st3 = st1 + st2;
    // constexpr auto start = Arrow::StaticStr::Find(st3, '2');
    // constexpr auto st4 = Arrow::StaticStr::SubStr<start, st3.length - start - 1>(st3);

    // std::cout << st1.data << std::endl;
    // std::cout << st2.data << std::endl;
    // std::cout << st3.data << std::endl;
    // std::cout << st4.data << std::endl;

    // constexpr auto s4 = Arrow::StrToType("123");

    // using str3Type = decltype(Arrow::StrToType(st3.data));
}