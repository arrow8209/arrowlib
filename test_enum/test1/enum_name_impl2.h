#pragma once
#include <type_traits>
#include <map>
#include <iostream>
#include "array_view.h"
#include "arrow/typelist/type_name/type_name_impl.h"
#include "arrow/typelist/static_string.h"

namespace Arrow2
{
using namespace Arrow;

namespace details
{

// 不同平台对应的关键子不一样 [zhuyb 2023-08-13 17:13:06]
#ifdef __clang__
constexpr char NoSocpedEnumNameStart[] = "t = ";
constexpr char NoSocpedEnumNameEnd[] = "]";
constexpr char NoSocpedEnumInValid[] = " = (";

constexpr char SocpedEnumNameStart[] = "::";
constexpr char SocpedEnumNameEnd[] = "]";
constexpr char SocpedEnumInValid[] = " = (";
#elif __GNUC__
constexpr char NoSocpedEnumNameStart[] = "t = ";
constexpr char NoSocpedEnumNameEnd[] = "]";
constexpr char NoSocpedEnumInValid[] = " = (";

constexpr char SocpedEnumNameStart[] = "::";
constexpr char SocpedEnumNameEnd[] = "]";
constexpr char SocpedEnumInValid[] = " = (";
#else
#endif

//判断枚举是 class enum 还是 enum
template<typename T, typename = typename std::enable_if<std::is_enum<T>::value>::type>
struct IsSocpedEnum : std::false_type {};

template<typename T>
struct IsSocpedEnum<T, typename std::enable_if<!std::is_convertible<T, int>::value>::type > : std::true_type {};

// 判断枚举值是否合法 [zhuyb 2023-08-13 17:13:55]
template<typename T, T t>
constexpr bool IsValid()
{
    return StaticStr::Find<0>(__PRETTY_FUNCTION__, details::NoSocpedEnumInValid) == std::string::npos;
}

// 获取限定性枚举 名称的长度 [zhuyb 2023-08-13 17:14:29]
template<typename T, T t>
constexpr size_t LengthScopedEnum()
{
    return StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameEnd) - StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameStart) - StaticStr::StrLen(SocpedEnumNameStart);
}

// 获取非限定枚举 名称的长度 [zhuyb 2023-08-13 17:30:38]
template<typename T, T t>
constexpr size_t LengthNoScopedEnum()
{
    return StaticStr::Find<0>(__PRETTY_FUNCTION__, NoSocpedEnumNameEnd) - StaticStr::Find<1>(__PRETTY_FUNCTION__, NoSocpedEnumNameStart) - StaticStr::StrLen(NoSocpedEnumNameStart);
}

// 获取长度 [zhuyb 2023-08-13 17:31:04]
template<typename T, T t>
constexpr size_t Length()
{
    return IsValid<T, t>() ? (details::IsSocpedEnum<T>::value ? details::LengthScopedEnum<T, t>() : details::LengthNoScopedEnum<T, t>()) : 0;
}

// 获取非限定枚举的名称 [zhuyb 2023-08-13 17:31:10]
template<typename T, T t>
constexpr auto NoSpcpedEnumItemName() -> StaticStr::StringView<Length<T, t>()>
{
    return StaticStr::StringView<Length<T, t>()>(__PRETTY_FUNCTION__ + StaticStr::Find<1>(__PRETTY_FUNCTION__, NoSocpedEnumNameStart) + StaticStr::StrLen(NoSocpedEnumNameStart),
                                         typename MakeIntegerSequence<Length<T, t>()>::type{});
}

// 获取限定枚举的名称 [zhuyb 2023-08-13 17:31:28]
template<typename T, T t>
constexpr auto SocpedEnumItemName() -> StaticStr::StringView<Length<T, t>()>
{
    return StaticStr::StringView<Length<T, t>()>(__PRETTY_FUNCTION__ + StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameStart) + StaticStr::StrLen(SocpedEnumNameStart),
                                         typename MakeIntegerSequence<Length<T, t>()>::type{});
}

}

// 调试使用 [zhuyb 2023-08-13 17:13:49]
template<typename T, T t>
void Trace()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

template<typename T, T t>
constexpr auto EnumItemName() -> StaticStr::StringView<details::Length<T, t>()>
{
    return  details::IsSocpedEnum<T>::value ? details::SocpedEnumItemName<T, t>() : details::NoSpcpedEnumItemName<T, t>();
}



// #define MAGIC_ENUM_FOR_EACH_256(T)                                                                                                                                                                 \
//   T(  0), T(  1), T(  2), T(  3), T(  4), T(  5), T(  6), T(  7), T(  8), T(  9), T( 10), T( 11), T( 12), T( 13), T( 14), T( 15), T( 16), T( 17), T( 18), T( 19), T( 20), T( 21), T( 22), T( 23), T( 24), T( 25), T( 26), T( 27), T( 28), T( 29), T( 30), T( 31), \
//   T( 32), T( 33), T( 34), T( 35), T( 36), T( 37), T( 38), T( 39), T( 40), T( 41), T( 42), T( 43), T( 44), T( 45), T( 46), T( 47), T( 48), T( 49), T( 50), T( 51), T( 52), T( 53), T( 54), T( 55), T( 56), T( 57), T( 58), T( 59), T( 60), T( 61), T( 62), T( 63), \
//   T( 64), T( 65), T( 66), T( 67), T( 68), T( 69), T( 70), T( 71), T( 72), T( 73), T( 74), T( 75), T( 76), T( 77), T( 78), T( 79), T( 80), T( 81), T( 82), T( 83), T( 84), T( 85), T( 86), T( 87), T( 88), T( 89), T( 90), T( 91), T( 92), T( 93), T( 94), T( 95), \
//   T( 96), T( 97), T( 98), T( 99), T(100), T(101), T(102), T(103), T(104), T(105), T(106), T(107), T(108), T(109), T(110), T(111), T(112), T(113), T(114), T(115), T(116), T(117), T(118), T(119), T(120), T(121), T(122), T(123), T(124), T(125), T(126), T(127), \
//   T(128), T(129), T(130), T(131), T(132), T(133), T(134), T(135), T(136), T(137), T(138), T(139), T(140), T(141), T(142), T(143), T(144), T(145), T(146), T(147), T(148), T(149), T(150), T(151), T(152), T(153), T(154), T(155), T(156), T(157), T(158), T(159), \
//   T(160), T(161), T(162), T(163), T(164), T(165), T(166), T(167), T(168), T(169), T(170), T(171), T(172), T(173), T(174), T(175), T(176), T(177), T(178), T(179), T(180), T(181), T(182), T(183), T(184), T(185), T(186), T(187), T(188), T(189), T(190), T(191), \
//   T(192), T(193), T(194), T(195), T(196), T(197), T(198), T(199), T(200), T(201), T(202), T(203), T(204), T(205), T(206), T(207), T(208), T(209), T(210), T(211), T(212), T(213), T(214), T(215), T(216), T(217), T(218), T(219), T(220), T(221), T(222), T(223), \
//   T(224), T(225), T(226), T(227), T(228), T(229), T(230), T(231), T(232), T(233), T(234), T(235), T(236), T(237), T(238), T(239), T(240), T(241), T(242), T(243), T(244), T(245), T(246), T(247), T(248), T(249), T(250), T(251), T(252), T(253), T(254), T(255)

#define ARROW_ENUM_FOR_EACH_1(T, startIndex, i, j) details::IsValid<T, static_cast<T>(startIndex + 0x##i##j)>()

#define ARROW_ENUM_FOR_EACH_16(T, startIndex, i)                                                \
    ARROW_ENUM_FOR_EACH_1(T, startIndex, 0, 0), ARROW_ENUM_FOR_EACH_1(T, startIndex, i, 1),        \
        ARROW_ENUM_FOR_EACH_1(T, startIndex, i, 2), ARROW_ENUM_FOR_EACH_1(T, startIndex, i, 3), \
        ARROW_ENUM_FOR_EACH_1(T, startIndex, i, 4), ARROW_ENUM_FOR_EACH_1(T, startIndex, i, 5), \
        ARROW_ENUM_FOR_EACH_1(T, startIndex, i, 6), ARROW_ENUM_FOR_EACH_1(T, startIndex, i, 7), \
        ARROW_ENUM_FOR_EACH_1(T, startIndex, i, 8), ARROW_ENUM_FOR_EACH_1(T, startIndex, i, 9), \
        ARROW_ENUM_FOR_EACH_1(T, startIndex, i, a), ARROW_ENUM_FOR_EACH_1(T, startIndex, i, b), \
        ARROW_ENUM_FOR_EACH_1(T, startIndex, i, c), ARROW_ENUM_FOR_EACH_1(T, startIndex, i, d), \
        ARROW_ENUM_FOR_EACH_1(T, startIndex, i, e), ARROW_ENUM_FOR_EACH_1(T, startIndex, i, f)

#define ARROW_ENUM_FOR_EACH_128(T, startIndex)                                              \
    ARROW_ENUM_FOR_EACH_16(T, startIndex, 0), ARROW_ENUM_FOR_EACH_16(T, startIndex, 1),     \
        ARROW_ENUM_FOR_EACH_16(T, startIndex, 2), ARROW_ENUM_FOR_EACH_16(T, startIndex, 3), \
        ARROW_ENUM_FOR_EACH_16(T, startIndex, 4), ARROW_ENUM_FOR_EACH_16(T, startIndex, 5), \
        ARROW_ENUM_FOR_EACH_16(T, startIndex, 6), ARROW_ENUM_FOR_EACH_16(T, startIndex, 7)

#define ARROW_ENUM_FOR_EACH_256(T, startIndex)                                              \
    ARROW_ENUM_FOR_EACH_16(T, startIndex, 0), ARROW_ENUM_FOR_EACH_16(T, startIndex, 1),     \
        ARROW_ENUM_FOR_EACH_16(T, startIndex, 2), ARROW_ENUM_FOR_EACH_16(T, startIndex, 3), \
        ARROW_ENUM_FOR_EACH_16(T, startIndex, 4), ARROW_ENUM_FOR_EACH_16(T, startIndex, 5), \
        ARROW_ENUM_FOR_EACH_16(T, startIndex, 6), ARROW_ENUM_FOR_EACH_16(T, startIndex, 7), \
        ARROW_ENUM_FOR_EACH_16(T, startIndex, 8), ARROW_ENUM_FOR_EACH_16(T, startIndex, 9), \
        ARROW_ENUM_FOR_EACH_16(T, startIndex, a), ARROW_ENUM_FOR_EACH_16(T, startIndex, b), \
        ARROW_ENUM_FOR_EACH_16(T, startIndex, c), ARROW_ENUM_FOR_EACH_16(T, startIndex, d), \
        ARROW_ENUM_FOR_EACH_16(T, startIndex, e), ARROW_ENUM_FOR_EACH_16(T, startIndex, f)

#define ARROW_ENUM_FOR_EACH_Impl(T, startIndex, StepSize) ARROW_ENUM_FOR_EACH_##StepSize(T, startIndex)
#define ARROW_ENUM_FOR_EACH(T, startIndex, StepSize) ARROW_ENUM_FOR_EACH_Impl(T, startIndex, StepSize)

#define EnumStepSize 128
using IntegerSequence_EnumStepSize = Arrow::MakeIntegerSequence<EnumStepSize>::type;

template<typename T, typename std::underlying_type<T>::type enumStart>
constexpr Array::ArrayView<bool, EnumStepSize> ValidCount()
{
    return Array::ArrayView<bool, EnumStepSize>(ARROW_ENUM_FOR_EACH(T, 0, EnumStepSize));
}


template<typename T, typename std::underlying_type<T>::type enumMin, typename std::underlying_type<T>::type enumMax>
constexpr Array::ArrayView<bool, (EnumStepSize + enumMax - enumMin) / EnumStepSize * EnumStepSize> ValidCount()
{
    // return (enumMin + EnumStepSize) >= enumMax ? ValidCount<T, enumMin>() : ValidCount<T, enumMin>() + ValidCount<T, enumMin + EnumStepSize, enumMax>();
    return true ? ValidCount<T, enumMin>() : ValidCount<T, enumMin>() + ValidCount<T, enumMin + EnumStepSize, enumMax>();
    // return ValidCount<T, enumMin>();
}

}