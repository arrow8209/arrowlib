#pragma once
#include <iostream>
#include <map>
#include <string>
#include <iostream>
#include "arrow/typelist/type_name.h"
#include "test1/enum_name_impl2.h"

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

enum class MyEnumA {
	MyEnumA_0 = 0
};

enum MyEnumB {
	MyEnumB_0 = 0
};

static void TestEnum()
{
    Arrow2::EnumItemName<MyEnumA, static_cast<MyEnumA>(0)>::Trace();
    Arrow2::EnumItemName<MyEnumB, static_cast<MyEnumB>(0)>::Trace();

    std::cout << Arrow2::EnumItemName<MyEnumA, static_cast<MyEnumA>(0)>::Length() << std::endl;
    std::cout << Arrow2::EnumItemName<MyEnumB, static_cast<MyEnumB>(0)>::Length() << std::endl;

    std::cout << Arrow2::EnumItemName<MyEnumA, static_cast<MyEnumA>(0)>::Impl().data << std::endl;
    std::cout << Arrow2::EnumItemName<MyEnumB, static_cast<MyEnumB>(0)>::Impl().data << std::endl;

    constexpr auto tmp = Arrow2::ValidData<10>();
    
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