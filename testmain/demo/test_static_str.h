#pragma once
#include <iostream>
#include "arrow/typelist/static_str/djb.h"
#include "arrow/typelist/static_str/string_view.h"
#include "arrow/typelist/static_string.h"
#include "arrow/typelist/static_string.h"



static constexpr auto ss1 = Arrow::StaticStr::Str("123");
static constexpr auto ss2 = Arrow::StaticStr::Str("4567");
static void TestStaticStr()
{
    using typeSS1 = STATIC_STRING(ss1.data);
    constexpr auto s1 = Arrow::StaticStr::Str("123");
    
    using typeS1= STATIC_STRING(s1.data);

    constexpr auto s2 = Arrow::StaticStr::Str("4567");
    constexpr auto s3 = s1 + s2;
    constexpr auto start = Arrow::StaticStr::Find<1>(s3, '2');

    std::cout << s1.data << std::endl;
    std::cout << s2.data << std::endl;
    std::cout << s3.data << std::endl;
    std::cout << start << std::endl;

    constexpr auto s4 = Arrow::StaticStr::SubStr<start>(s3.data);
    constexpr auto s5 = Arrow::StaticStr::SubStr<2>(s3);


    std::cout << s4.data << std::endl;
    std::cout << s5.data << std::endl;
    
    std::cout << Arrow::StaticStr::Find(s3.data, "23") << std::endl;
    std::cout << Arrow::StaticStr::Find(s3.data, "34") << std::endl;
    std::cout << Arrow::StaticStr::Find<2>(s3.data, "23") << std::endl;

    std::cout << Arrow::StaticStr::StrLen(s1.data) << ":" << s1.data << std::endl;
    std::cout << Arrow::StaticStr::StrLen(s2.data) << ":" << s2.data << std::endl;
    std::cout << Arrow::StaticStr::StrLen(s3.data) << ":" << s3.data << std::endl;
    std::cout << Arrow::StaticStr::StrLen(s4.data) << ":" << s4.data << std::endl;
    std::cout << Arrow::StaticStr::StrLen(s5.data) << ":" << s5.data << std::endl;
    // constexpr auto s3 = s1+s2;
}

static void TestStrDjb2()
{
    // constexpr auto value = Arrow::StaticStr::StrToUInt32(Arrow::StaticStr::Str("123") + Arrow::StaticStr::Str("45") + Arrow::StaticStr::Str("const char (&a)[length]") + Arrow::StaticStr::Str("abcde"));
    constexpr auto value = Arrow::StaticStr::StrToUInt32(Arrow::StaticStr::Str("const char (&a)[length]"));
    std::cout << value << std::endl;
    
}