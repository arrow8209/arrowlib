#pragma once
#include <iostream>
#include <map>
#include <string>
#include <iostream>
#include "arrow/typelist/type_name.h"
#include "test1/enum_name_impl2.h"
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
    MyEnumA_220 = 270
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
    Arrow2::details::Trace<MyEnumA, static_cast<MyEnumA>(0)>();
    Arrow2::details::Trace<MyEnumA, static_cast<MyEnumA>(1)>();
    Arrow2::details::Trace<MyEnumB, static_cast<MyEnumB>(0)>();
    Arrow2::details::Trace<MyEnumB, static_cast<MyEnumB>(1)>();
    std::cout << Arrow2::details::IsValid<MyEnumA, static_cast<MyEnumA>(0)>() << std::endl;
    std::cout << Arrow2::details::IsValid<MyEnumA, static_cast<MyEnumA>(1)>() << std::endl;
    std::cout << Arrow2::details::IsValid<MyEnumB, static_cast<MyEnumB>(0)>() << std::endl;
    std::cout << Arrow2::details::IsValid<MyEnumB, static_cast<MyEnumB>(1)>() << std::endl;

    std::cout << Arrow2::details::Valid<MyEnumA, static_cast<MyEnumA>(0)>::value << std::endl;
    std::cout << Arrow2::details::Valid<MyEnumA, static_cast<MyEnumA>(1)>::value << std::endl;
    Arrow2::details::EnumItemStrView<MyEnumA, static_cast<MyEnumA>(0)>::TraceLengthScopedEnum();
    // Arrow2::details::TraceLengthScopedEnum<MyEnumA, static_cast<MyEnumA>(0)>();
    // Arrow2::details::TraceLengthNoScopedEnum<MyEnumB, static_cast<MyEnumB>(1)>();
    // Arrow2::details::TraceLengthNoScopedEnum<MyEnumB, static_cast<MyEnumB>(0)>();
    constexpr auto tmpA =Arrow2::details::EnumItemStrView<MyEnumA, static_cast<MyEnumA>(0)>::value;
    constexpr auto tmpB =Arrow2::EnumItemArrayInterval<MyEnumA, 0, 20>::Array();
    std::cout << tmpA.data << std::endl;
    // std::cout << TTmp<MyEnumA, static_cast<MyEnumA>(1)>::Str() << std::endl;
}