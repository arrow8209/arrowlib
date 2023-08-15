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

static void TestEnum()
{
    Arrow2::Trace<MyEnumA, static_cast<MyEnumA>(0)>();
    Arrow2::Trace<MyEnumB, static_cast<MyEnumB>(0)>();

    // Arrow2::details::TraceLengthScopedEnum<MyEnumA, static_cast<MyEnumA>(0)>();

    // Arrow2::details::TraceLengthNoScopedEnum<MyEnumB, static_cast<MyEnumB>(0)>();
    // std::cout << Arrow2::details::Length<MyEnumA, static_cast<MyEnumA>(0)>() << std::endl;
    // Arrow2::details::TraceLengthScopedEnum<MyEnumA, static_cast<MyEnumA>(0)>();

    // std::cout << std::endl;
    // std::cout << Arrow2::details::Length<MyEnumB, static_cast<MyEnumB>(0)>() << std::endl;
    // Arrow2::details::TraceLengthNoScopedEnum<MyEnumB, static_cast<MyEnumB>(0)>();

    // auto itemName1 = Arrow2::EnumItemName<MyEnumA, static_cast<MyEnumA>(0)>();
    // std::cout << itemName1.data << std::endl;

    // auto itemName2 = Arrow2::EnumItemName<MyEnumB, static_cast<MyEnumB>(0)>();
    // std::cout << itemName2.data << std::endl;

    // std::cout << Arrow2::details::IsSocpedEnum<MyEnumA>::value << std::endl;
    // std::cout << Arrow2::details::IsSocpedEnum<MyEnumB>::value << std::endl;

    // constexpr Arrow2::Array::ArrayView<bool, 2> tmp1(true, false);
    // constexpr Arrow2::Array::ArrayView<bool, 0> tmp2;
    // auto t3 = tmp1 + tmp2;

    // constexpr auto tmp1 = Arrow2::ValidArrayInOneStepSize<MyEnumA, 0>::array;
    // constexpr auto tmp2 = Arrow2::ValidArray<MyEnumA, EnumStepSize, 0>::array;
    // auto t3 = tmp1 + tmp2;
    // constexpr auto tmp1 = Arrow2::EnumItemInfo<MyEnumA, 0>::itemView;
    
    // std::cout << Arrow2::ItemName<MyEnumA, 0>.data << std::endl;
    // constexpr auto tmpA = Arrow2::MyEnumName1<MyEnumA, 0, 129>();
    // constexpr auto tmpA0 = tmpA[0];
    // int i = 1;
    // auto tmpA1 = tmpA[1];
    // constexpr auto len = Arrow2::MyEnumName2<MyEnumA, 0, 129>();
    constexpr auto tmpA = Arrow2::ValidInOneStepSize<MyEnumA, 0>::validTmp;
    constexpr auto tmpB = Arrow2::ValidArrayInOneStepSize<MyEnumA, 0>::array;
    constexpr auto tmpC = Arrow2::ValidArray<MyEnumA, 0, 258>::validArray;
    MyEnumA enumA = MyEnumA::MyEnumA_20;
    auto tmpD = Arrow2::ValidItemName<MyEnumA, 0, 258>::ItemName(enumA);
    // constexpr auto tmpD = Arrow2::EnumItemName<MyEnumA, tmpC[0]>;
    // constexpr auto tmpE = tmpD.data;

    constexpr auto tmpA1 = Arrow2::ValidInOneStepSize<MyEnumB, 0>::validTmp;
    constexpr auto tmpB1 = Arrow2::ValidArrayInOneStepSize<MyEnumB, 0>::array;
    constexpr auto tmpC1 = Arrow2::ValidArray<MyEnumB, 0 , 258>::validArray;
    // constexpr auto tmpD1 = Arrow2::ValidArray<MyEnumB, 0 , 258>::itemNameArray;
    // constexpr auto tmpB = Arrow2::ValidArray<MyEnumA, 0, 100>::array;

    // constexpr auto tmp1 = Arrow2::ValidCount<MyEnumA, 0>();
    // constexpr auto tmp2 = Arrow2::ValidCount<MyEnumA, 0>();
    // auto t3 = tmp1 + tmp2 + tmp1;

    // FunName<MyEnumA, static_cast<MyEnumA>(0)>();
    // FunName<MyEnumA, static_cast<MyEnumA>(1)>();

    // Arrow2::EnumItemName<MyEnumA, MyEnumA(1)>::Trace<MyEnumA, a>();
    // Arrow2::EnumItemName<MyEnumB, MyEnumB(2)>::Trace<MyEnumB, b>();

    // std::cout << typeid(static_cast<MyEnumA>(2)).name() << std::endl;
    // std::cout << typeid(static_cast<MyEnumB>(2)).name() << std::endl;
    // std::cout << Arrow2::EnumItemName<MyEnumA, static_cast<MyEnumA>(0)>::Length() << std::endl;
    // std::cout << Arrow2::EnumItemName<MyEnumB, static_cast<MyEnumB>(0)>::Length() << std::endl;

    // std::cout << Arrow2::EnumItemName<MyEnumA, static_cast<MyEnumA>(0)>::Impl().data << std::endl;
    // std::cout << Arrow2::EnumItemName<MyEnumB, static_cast<MyEnumB>(0)>::Impl().data << std::endl;

    // constexpr auto tmp = Arrow2::ValidData<10>();
    
    // tmp.SetValidData<MyEnumA, 0>();
    // int n = 5;
    // Arrow2::EnumItemName<MyTestEnum, static_cast<MyTestEnum>(4)>::Trace();
    // Arrow2::EnumItemName<MyTestEnum, static_cast<MyTestEnum>(1)>::Trace();

    // std::cout << "Index:" << Arrow2::EnumItemName<MyTestEnum, static_cast<MyTestEnum>(4)>::IsValid() << std::endl;
    // std::cout << "Index:" << Arrow2::EnumItemName<MyTestEnum, static_cast<MyTestEnum>(5)>::IsValid() << std::endl;

    // std::cout << Arrow2::EnumItemName<MyTestEnum, static_cast<MyTestEnum>(4)>::Impl().data << std::endl;
    // std::cout << Arrow2::EnumItemName<MyTestEnum, static_cast<MyTestEnum>(1)>::Impl().data << std::endl;

    // std::cout << Arrow2::EnumItemName<MyTestEnum, static_cast<MyTestEnum>(4)>::Impl().data << std::endl;
    // std::cout << Arrow2::EnumItemName<MyTestEnum, static_cast<MyTestEnum>(1)>::Impl().data << std::endl;

    // std::cout << Arrow2::EnumItemName<MyTestEnum, static_cast<MyTestEnum>(4)>::FullName() << std::endl;
    // std::cout << Arrow2::EnumItemName<MyTestEnum, static_cast<MyTestEnum>(5)>::FullName() << std::endl;
}