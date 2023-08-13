#pragma once
#include <type_traits>
#include <map>
#include <iostream>
#include "arrow/typelist/type_name/type_name_impl.h"
#include "arrow/typelist/static_string.h"

namespace Arrow3
{
using namespace Arrow;
namespace details
{

// 不同平台对应的关键子不一样 [zhuyb 2023-08-13 17:13:06]
#ifdef __clang__
constexpr char NoSocpedEnumNameStart[] = "t = ";
constexpr char NoSocpedEnumNameEnd[] = "]";
constexpr char NoSocpedEnumInVain[] = " = (";

constexpr char SocpedEnumNameStart[] = "::";
constexpr char SocpedEnumNameEnd[] = "]";
constexpr char SocpedEnumInVain[] = " = (";
#elif __GNUC__
constexpr char NoSocpedEnumNameStart[] = "t = ";
constexpr char NoSocpedEnumNameEnd[] = "]";
constexpr char NoSocpedEnumInVain[] = " = (";

constexpr char SocpedEnumNameStart[] = "::";
constexpr char SocpedEnumNameEnd[] = "]";
constexpr char SocpedEnumInVain[] = " = (";
#else
#endif

//判断枚举是 class enum 还是 enum
template<typename T, typename = typename std::enable_if<std::is_enum<T>::value>::type>
struct IsSocpedEnum : std::false_type {};

template<typename T>
struct IsSocpedEnum<T, typename std::enable_if<!std::is_convertible<T, int>::value>::type > : std::true_type {};

}

// 调试使用 [zhuyb 2023-08-13 17:13:49]
template<typename T, T t>
void Trace()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

// 判断枚举值是否合法 [zhuyb 2023-08-13 17:13:55]
template<typename T, T t>
constexpr bool IsValid()
{
    return StaticStr::Find<0>(__PRETTY_FUNCTION__, details::NoSocpedEnumInVain) == std::string::npos;
}

//  [zhuyb 2023-08-13 17:14:29]
template<typename T, T t>
constexpr size_t LengthScopedEnum()
{
    return StaticStr::Find<0>(__PRETTY_FUNCTION__, details::SocpedEnumNameEnd) - StaticStr::Find<0>(__PRETTY_FUNCTION__, details::SocpedEnumNameStart) - StaticStr::StrLen(details::SocpedEnumNameStart);
}

template<typename T, T t>
constexpr size_t LengthNoScopedEnum()
{
    return StaticStr::Find<0>(__PRETTY_FUNCTION__, details::NoSocpedEnumNameEnd) - StaticStr::Find<1>(__PRETTY_FUNCTION__, details::NoSocpedEnumNameStart) - StaticStr::StrLen(details::NoSocpedEnumNameStart);
}

template<typename T, T t>
constexpr size_t Length()
{
    return IsValid<T, t>() ? (details::IsSocpedEnum<T>::value ? LengthScopedEnum<T, t>() : LengthNoScopedEnum<T, t>()) : 0;
}


// template<typename T, T t, size_t length>
// StaticStr::StringView<length> EnumItemNameTrace()
// {
//     std::cout << __PRETTY_FUNCTION__ << std::endl;
//     return StaticStr::Str("12345");
// }

// template<typename T, T t, size_t length>
// constexpr StaticStr::StringView<length> EnumItemName()
// {
//     return StaticStr::StringView<length>(__PRETTY_FUNCTION__ + StaticStr::Find<1>(__PRETTY_FUNCTION__, details::NoSocpedEnumNameStart) + StaticStr::StrLen(details::NoSocpedEnumNameStart),
//                                          typename MakeIntegerSequence<length>::type{});
// }

template<typename T, T t>
constexpr auto NoSpcpedEnumItemName() -> StaticStr::StringView<Length<T, t>()>
{
    return StaticStr::StringView<Length<T, t>()>(__PRETTY_FUNCTION__ + StaticStr::Find<1>(__PRETTY_FUNCTION__, details::NoSocpedEnumNameStart) + StaticStr::StrLen(details::NoSocpedEnumNameStart),
                                         typename MakeIntegerSequence<Length<T, t>()>::type{});
}

template<typename T, T t>
constexpr auto SocpedEnumItemName() -> StaticStr::StringView<Length<T, t>()>
{
    return StaticStr::StringView<Length<T, t>()>(__PRETTY_FUNCTION__ + StaticStr::Find<0>(__PRETTY_FUNCTION__, details::SocpedEnumNameStart) + StaticStr::StrLen(details::SocpedEnumNameStart),
                                         typename MakeIntegerSequence<Length<T, t>()>::type{});
}

template<typename T, T t>
constexpr auto EnumItemName() -> StaticStr::StringView<Length<T, t>()>
{
    // return EnumItemName(typename details::IsSocpedEnum<T>::type{});
    return  details::IsSocpedEnum<T>::value ? SocpedEnumItemName<T, t>(std::false_type{}) : EnumItemName<T, t>(std::true_type{});
}

// // 核心代码用于获取枚举类型 [zhuyb 2023-08-04 11:29:37]
// template<typename T, T t, int length = -1>
// struct EnumItemName
// {
//     static void Trace()
//     {
//         std::cout << __PRETTY_FUNCTION__ << std::endl;
//     }

//     constexpr static StaticStr::StringView<length> Impl()
//     {
//         return details::IsSocpedEnum<T>::value ? SocpedEnumName() : NoSocpedEnumName();
//     }

//     constexpr static StaticStr::StringView<length> NoSocpedEnumName()
//     {
//         return StaticStr::StringView<length>(__PRETTY_FUNCTION__ + StaticStr::Find<1>(__PRETTY_FUNCTION__, details::NoSocpedEnumNameStart) + StaticStr::StrLen(details::NoSocpedEnumNameStart),
//                                              typename MakeIntegerSequence<length>::type{});
//     }

//     constexpr static StaticStr::StringView<length> SocpedEnumName()
//     {
//         return StaticStr::StringView<length>(__PRETTY_FUNCTION__ + StaticStr::Find<0>(__PRETTY_FUNCTION__, details::SocpedEnumNameStart) + StaticStr::StrLen(details::SocpedEnumNameStart),
//                                              typename MakeIntegerSequence<length>::type{});
//     }
// };

// // 错误的枚举值 [zhuyb 2023-08-04 11:29:55]
// template<typename T, T t>
// struct EnumItemName<T, t, 0>
// {
//     static void Trace()
//     {
//         std::cout << __PRETTY_FUNCTION__ << std::endl;
//     }

//     constexpr static StaticStr::StringView<0> Impl()
//     {
//         return StaticStr::Str("");
//     }
// };

// // 最初的入口 用于计算枚举字符串的长度 [zhuyb 2023-08-04 11:30:17]
// template<typename T, T t>
// struct EnumItemName<T, t, -1>
// {
//     template<typename T2, T2 t2>
//     static void Trace()
//     {
//         std::cout << __PRETTY_FUNCTION__ << std::endl;
//         EnumItemName<T, t, Length()>::Trace();
//     }

//     /**
//      * @description: 判断值是否合法
//      * @return {*}
//      */
//     constexpr static bool IsValid()
//     {
//         return StaticStr::Find<0>(__PRETTY_FUNCTION__, details::NoSocpedEnumInVain) == std::string::npos;
//     }

//     /**
//      * @description: 获取枚举类型对应字符串的长度
//      * @return {*}
//      */
//     constexpr static int Length()
//     {
//         return IsValid() ? (details::IsSocpedEnum<T>::value ? LengthScopedEnum() : LengthNoScopedEnum()) : 0;
//     }
    
//     constexpr static int LengthScopedEnum()
//     {
//         return StaticStr::Find<0>(__PRETTY_FUNCTION__, details::SocpedEnumNameEnd) - StaticStr::Find<0>(__PRETTY_FUNCTION__, details::SocpedEnumNameStart) - StaticStr::StrLen(details::SocpedEnumNameStart);
//     }

//     constexpr static int LengthNoScopedEnum()
//     {
//         return StaticStr::Find<0>(__PRETTY_FUNCTION__, details::NoSocpedEnumNameEnd) - StaticStr::Find<1>(__PRETTY_FUNCTION__, details::NoSocpedEnumNameStart) - StaticStr::StrLen(details::NoSocpedEnumNameStart);
//     }

//     /**
//      * @description: 获取枚举值对应字符串
//      * @return {*}
//      */    
//     constexpr static auto Impl() -> decltype(EnumItemName<T, t, EnumItemName<T, t>::Length()>::Impl())
//     {
//         return EnumItemName<T, t, EnumItemName<T, t>::Length()>::Impl();
//     }

//     // constexpr static auto Impl1() -> decltype(TypeName<T>::Impl() + StaticStr::Str("::") + EnumItemName<T, t>::Impl())
//     // {
//     //     return TypeName<T>::Impl() + StaticStr::Str("::") + EnumItemName<T, t>::Impl();
//     // }

//     constexpr static const char* Name()
//     {
//         return value.data;
//     }

//     // constexpr static const char* FullName()
//     // {
//     //     return long_value.data;
//     // }

//     static constexpr decltype(EnumItemName<T, t>::Impl()) value = EnumItemName<T, t>::Impl();
//     // static constexpr decltype(EnumItemName<T, t>::Impl1()) long_value = EnumItemName<T, t>::Impl1();

// };

}