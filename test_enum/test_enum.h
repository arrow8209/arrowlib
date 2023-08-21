#pragma once
#include <iostream>
#include <map>
#include <string>
#include <iostream>
#include "arrow/typelist/type_name.h"
#include "test1/enum_name_impl2.h"
#include "test1/type_name_assist.h"
// #include "test1/enum_name_impl3.h"

// enum class MyTestEnum {
// 	MyTestEnum_0 = 0,
// 	MyTestEnum_1 = 10,
// 	MyTestEnum_2 = 12,
// 	MyTestEnum_3 = 20,
// 	MyTestEnum_4 = 22,
// 	MyTestEnum_5 = 50,
// 	MyTestEnum_6 = 52,
// 	MyTestEnum_7 = 70,
// 	MyTestEnum_8 = 8,
// 	MyTestEnum_9 = 90,
// 	MyTestEnum_10 = 910,
// 	MyTestEnum_11 = 111,
// 	MyTestEnum_12 = 112,
// 	MyTestEnum_13 = 13
// };

enum class MyTestEnum {
	MyTestEnum_0 = 0,
	MyTestEnum_1 = 1,
	// MyTestEnum_2 = 3,
	MyTestEnum_3 = 2,
	// MyTestEnum_4 = 4,
	MyTestEnum_5 = 5,
	// MyTestEnum_6 = 9,
	MyTestEnum_7 = 8,
	// MyTestEnum_8 = 7,
	MyTestEnum_9 = 6,
	MyTestEnum_10 = 10,
	// MyTestEnum_11 = 11,
	MyTestEnum_12 = 12,
	MyTestEnum_13 = 13
};

enum class MyEnumA
{
    MyEnumA_0 = 0,
    MyEnumA_20 = 20,
    MyEnumA_50 = 50,
    MyEnumA_70 = 70,
    MyEnumA_220 = 220
};

enum MyEnumB {
	MyEnumB_0 = 0
};

template<typename T, T val>
void FunName()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

constexpr MyEnumA a= static_cast<MyEnumA>(1);
constexpr MyEnumB b= static_cast<MyEnumB>(1);

#define V_TO_STR_IMPL(val) #val
#define V_TO_STR(val) V_TO_STR_IMPL(val)
template<typename T, T val>
struct TTmp
{
    static constexpr const char* Str()
    {
        return V_TO_STR(1);
    }
};

static void TestEnum()
{
    // constexpr auto tmpS = Arrow::StaticStr::NumToStr<uint32_t, 10>();

    // 测试枚举的合法性 [zhuyb 2023-08-21 11:00:26]
    std::cout << Arrow2::details::IsValid<MyEnumA, static_cast<MyEnumA>(0)>() << std::endl;
    std::cout << Arrow2::details::IsValid<MyEnumA, static_cast<MyEnumA>(1)>() << std::endl;
    std::cout << Arrow2::details::IsValid<MyEnumB, static_cast<MyEnumB>(0)>() << std::endl;
    std::cout << Arrow2::details::IsValid<MyEnumB, static_cast<MyEnumB>(1)>() << std::endl;


    // // 测试枚举名称 [zhuyb 2023-08-21 11:00:37]
    constexpr auto tmp1 = Arrow2::details::EnumItemStrView<MyEnumA, static_cast<MyEnumA>(0)>::strView;
    constexpr auto tmp2 = Arrow2::details::EnumItemStrView<MyEnumA, static_cast<MyEnumA>(1)>::strView;
    constexpr auto tmp3 = Arrow2::details::EnumItemStrView<MyEnumB, static_cast<MyEnumB>(0)>::strView;
    constexpr auto tmp4 = Arrow2::details::EnumItemStrView<MyEnumB, static_cast<MyEnumB>(1)>::strView;
    std::cout << tmp1.data << std::endl;
    std::cout << tmp2.data << std::endl;
    std::cout << tmp3.data << std::endl;
    std::cout << tmp4.data << std::endl;

    // 测试枚举值单区间列表 [zhuyb 2023-08-21 11:00:
    constexpr auto tmp5 = Arrow2::details::EnumItemArrayInterval<MyEnumA, 0, 250>::Array();

    //  [zhuyb 2023-08-21 21:17:22]
    constexpr auto tmp6 = Arrow2::EnumItemArray<MyEnumA, MyEnumA::MyEnumA_0>::array;
    constexpr auto tmp7 = Arrow2::EnumItemArray<MyEnumA, MyEnumA::MyEnumA_0, MyEnumA::MyEnumA_20,MyEnumA::MyEnumA_50, MyEnumA::MyEnumA_70>::Array();
    using MyEnumAStr = Arrow2::EnumItemToStr<MyEnumA, MyEnumA::MyEnumA_0, MyEnumA::MyEnumA_20,MyEnumA::MyEnumA_50, MyEnumA::MyEnumA_70>;
    // constexpr auto tmp8 = MyEnumAStr::Str(MyEnumA::MyEnumA_0);

    for(int i = -10; i < 100; i++)
    {
        MyEnumA tmp =static_cast<MyEnumA>(i);
        constexpr auto pStr = MyEnumAStr::Str(tmp);
        std::cout << i << ":" << pStr << std::endl;
    }

}