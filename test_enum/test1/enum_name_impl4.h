#pragma once
#include <type_traits>
#include <map>
#include <iostream>
#include "arrow/typelist/type_name/type_name_impl.h"
#include "arrow/typelist/static_array.h"
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
constexpr char NoSocpedEnumNameStart[] = "T t = ";
constexpr char NoSocpedEnumNameEnd[] = ";";
constexpr char NoSocpedEnumInValid[] = " = (";

constexpr char SocpedEnumNameStart[] = "::";
constexpr char SocpedEnumNameEnd[] = ";";
constexpr char SocpedEnumInValid[] = " = (";
#else
#endif

//判断枚举是 class enum 还是 enum
template<typename T, typename = typename std::enable_if<std::is_enum<T>::value>::type>
struct IsSocpedEnum : std::false_type {};

template<typename T>
struct IsSocpedEnum<T, typename std::enable_if<!std::is_convertible<T, int>::value>::type > : std::true_type {};

// template <typename T, T t>
// struct Valid : public std::false_type{};

// 判断枚举值是否合法 [zhuyb 2023-08-13 17:13:55]
template<typename T, T t>
constexpr bool IsValid()
{
    return StaticStr::Find<0>(__PRETTY_FUNCTION__, details::NoSocpedEnumInValid) == std::string::npos;
}

template <typename T, T t, typename SFINEA = void>
struct Valid : public std::false_type
{
};

template <typename T, T t>
struct Valid<T, t, typename std::enable_if<IsValid<T, t>()>::type> : public std::false_type{};

// 获取限定性枚举 名称的长度 gcc 在处理size_t会自动添加模板参数列表中添加 size_t = long unsigned int  [zhuyb 2023-08-13 17:14:29]
// gcc 输出格式：size_t Arrow2::LengthScopedEnum() [with T = Enum...; T t = Enum...; size_t = long unsigned int]
// clang 输出格式：size_t Arrow2::LengthScopedEnum() [with T = Enum...; T t = Enum...; size_t = long unsigned int]
template<typename T, T t>
constexpr size_t LengthScopedEnum()
{
    return StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameEnd) - StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameStart) - StaticStr::StrLen(SocpedEnumNameStart);
}

// 测试使用 [zhuyb 2023-08-14 13:42:33]
template<typename T, T t>
size_t TraceLengthScopedEnum()
{
    std::cout << "ScopedEnum" << std::endl;
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    std::cout << "EndStr:\"" << SocpedEnumNameEnd << "\""
              << " End Last Index:" << StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameEnd) << std::endl;
    std::cout << "StartStr:\"" << SocpedEnumNameStart  << "\"" << "Start Last Index:" << StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameStart) << std::endl;
    std::cout << "StartCharLen::" << StaticStr::StrLen(SocpedEnumNameStart) << std::endl;
    std::cout << "Lenth1:" << StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameEnd) - StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameStart) - StaticStr::StrLen(SocpedEnumNameStart)<< std::endl;
    std::cout << "Length2:" << LengthScopedEnum<T, t>() << std::endl;
    return 0;
}

// 获取非限定枚举 名称的长度 [zhuyb 2023-08-13 17:30:38]
template<typename T, T t>
constexpr size_t LengthNoScopedEnum()
{
    return StaticStr::Find<0>(__PRETTY_FUNCTION__, NoSocpedEnumNameEnd) - StaticStr::Find<1>(__PRETTY_FUNCTION__, NoSocpedEnumNameStart) - StaticStr::StrLen(NoSocpedEnumNameStart);
}

// 测试使用 [zhuyb 2023-08-14 15:19:20]
template<typename T, T t>
size_t TraceLengthNoScopedEnum()
{
    std::cout << "NoScopedEnum" << std::endl;
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    std::cout << "EndStr:\"" << NoSocpedEnumNameEnd  << "\"" << " End Last Index:" << StaticStr::Find<0>(__PRETTY_FUNCTION__, NoSocpedEnumNameEnd) << std::endl;
    std::cout << "StartStr:\"" << NoSocpedEnumNameStart  << "\"" << "Start First Index:" << StaticStr::Find<1>(__PRETTY_FUNCTION__, NoSocpedEnumNameStart) << std::endl;
    std::cout << "StartCharLen:" << StaticStr::StrLen(NoSocpedEnumNameStart) << std::endl;
    std::cout << "Length1:" << StaticStr::Find<0>(__PRETTY_FUNCTION__, NoSocpedEnumNameEnd) - StaticStr::Find<1>(__PRETTY_FUNCTION__, NoSocpedEnumNameStart) - StaticStr::StrLen(NoSocpedEnumNameStart) << std::endl;
    std::cout << "Length2:" << LengthNoScopedEnum<T, t>() << std::endl;
    return 0;
}

// 获取长度 [zhuyb 2023-08-13 17:31:04]
template<typename T, T t>
constexpr size_t Length()
{
    return IsValid<T, t>() ? (details::IsSocpedEnum<T>::value ? details::LengthScopedEnum<T, t>() : details::LengthNoScopedEnum<T, t>()) : 0;
}

// 获取长度 [zhuyb 2023-08-13 17:31:04]
template<typename T, T t>
void TraceLength()
{
    details::IsSocpedEnum<T>::value ? details::TraceLengthScopedEnum<T, t>() : details::TraceLengthNoScopedEnum<T, t>();
}

// 获取非限定枚举的名称 [zhuyb 2023-08-13 17:31:10]
template<typename T, T t>
constexpr auto NoSpcpedEnumItemName() -> StaticStr::StringView<Length<T, t>()>
{
    return StaticStr::StringView<Length<T, t>()>(__PRETTY_FUNCTION__ + StaticStr::Find<1>(__PRETTY_FUNCTION__, NoSocpedEnumNameStart) + StaticStr::StrLen(NoSocpedEnumNameStart));
}

// 获取限定枚举的名称 [zhuyb 2023-08-13 17:31:28]
template<typename T, T t>
constexpr auto SocpedEnumItemName() -> StaticStr::StringView<Length<T, t>()>
{
    return StaticStr::StringView<Length<T, t>()>(__PRETTY_FUNCTION__ + StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameStart) + StaticStr::StrLen(SocpedEnumNameStart));
}

// // 获取枚举短名称(不包括限定域名称) [zhuyb 2023-08-14 15:19:38]
// template<typename T, T t>
// constexpr auto EnumItemNameImpl() -> StaticStr::StringView<details::Length<T, t>()>
// {
//     return  details::IsSocpedEnum<T>::value ? details::SocpedEnumItemName<T, t>() : details::NoSpcpedEnumItemName<T, t>();
// }

template<typename T, T v, typename SFNIEA = typename details::Valid<T, v>::type>
struct EnumItemName{};

template<typename T, T v>
struct EnumItemName<T, v, std::true_type>
{

    static constexpr auto Impl() -> StaticStr::StringView<details::Length<T, v>()>
    {
        return IsValid<T, v>() ? details::IsSocpedEnum<T>::value ? details::SocpedEnumItemName<T, v>() : details::NoSpcpedEnumItemName<T, v>() : StaticStr::StringView<0>();
    }

    static constexpr decltype(Impl()) itemView = Impl();
};
template<typename T, T v>
constexpr decltype(EnumItemName<T, v,std::true_type>::Impl()) EnumItemName<T, v, std::true_type>::itemView;

template<typename T, T v>
struct EnumItemName<T, v, std::false_type>
{
    using valueType = typename std::underlying_type<T>::type;
    static constexpr auto Impl() -> decltype(Arrow::TypeName<T>::Impl() + StaticStr::Str(" undefined:") + StaticStr::NumToStr<valueType, static_cast<valueType>(v)>()) 
    {
        return Arrow::TypeName<T>::Impl() + StaticStr::Str(" undefined:") + StaticStr::NumToStr<valueType, static_cast<valueType>(v)>();
    }

    static constexpr decltype(EnumItemName<T, v, std::false_type>::Impl()) itemView = EnumItemName<T, v, std::false_type>::Impl();
};
template <typename T, T v>
constexpr decltype(EnumItemName<T, v, std::false_type>::Impl()) EnumItemName<T, v, std::false_type>::itemView;
}
// 调试使用 [zhuyb 2023-08-13 17:13:49]
template<typename T, T t>
size_t Trace()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return 0;
}

#define IS_VALID(T, startIndex, i, j) details::IsValid<T, static_cast<T>(startIndex + 0x##i##j)>()

#define ENUM_ITEM_NAME(T, startIndex, i, j) details::EnumItemName<T, static_cast<T>(startIndex + 0x##i##j)>::itemView.data

#define ENUM_ITEM_INFO(T, startIndex, i, j) EnumItemInfo<T>(static_cast<T>(startIndex + 0x##i##j), ENUM_ITEM_NAME(T, startIndex, i, j))
    // using __EnumItemInfo = EnumItemInfo<T>;
    // using __EnumItemName = details::EnumItemName<T, static_cast<T>(enumStart)>;
// __EnumItemInfo(static_cast<T>(enumStart), __EnumItemName::itemView.data)

#define ARROW_ENUM_FOR_EACH_1(F, T, startIndex, i, j) F(T, startIndex, i,j)

#define ARROW_ENUM_FOR_EACH_X(F, T, startIndex, i)                                                \
    ARROW_ENUM_FOR_EACH_1(F, T, startIndex, i, 0), ARROW_ENUM_FOR_EACH_1(F, T, startIndex, i, 1),        \
        ARROW_ENUM_FOR_EACH_1(F, T, startIndex, i, 2), ARROW_ENUM_FOR_EACH_1(F, T, startIndex, i, 3), \
        ARROW_ENUM_FOR_EACH_1(F, T, startIndex, i, 4), ARROW_ENUM_FOR_EACH_1(F, T, startIndex, i, 5), \
        ARROW_ENUM_FOR_EACH_1(F, T, startIndex, i, 6), ARROW_ENUM_FOR_EACH_1(F, T, startIndex, i, 7), \
        ARROW_ENUM_FOR_EACH_1(F, T, startIndex, i, 8), ARROW_ENUM_FOR_EACH_1(F, T, startIndex, i, 9), \
        ARROW_ENUM_FOR_EACH_1(F, T, startIndex, i, a), ARROW_ENUM_FOR_EACH_1(F, T, startIndex, i, b), \
        ARROW_ENUM_FOR_EACH_1(F, T, startIndex, i, c), ARROW_ENUM_FOR_EACH_1(F, T, startIndex, i, d), \
        ARROW_ENUM_FOR_EACH_1(F, T, startIndex, i, e), ARROW_ENUM_FOR_EACH_1(F, T, startIndex, i, f)

#define ARROW_ENUM_FOR_EACH_16(F, T, startIndex) ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 0)      

#define ARROW_ENUM_FOR_EACH_32(F, T, startIndex) ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 0), ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 1)

#define ARROW_ENUM_FOR_EACH_64(F, T, startIndex) ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 0),ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 1), \
    ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 2),ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 3)

#define ARROW_ENUM_FOR_EACH_128(F, T, startIndex)                                              \
    ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 0), ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 1),     \
        ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 2), ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 3), \
        ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 4), ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 5), \
        ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 6), ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 7)

#define ARROW_ENUM_FOR_EACH_256(F, T, startIndex)                                              \
    ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 0), ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 1),     \
        ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 2), ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 3), \
        ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 4), ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 5), \
        ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 6), ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 7), \
        ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 8), ARROW_ENUM_FOR_EACH_X(F, T, startIndex, 9), \
        ARROW_ENUM_FOR_EACH_X(F, T, startIndex, a), ARROW_ENUM_FOR_EACH_X(F, T, startIndex, b), \
        ARROW_ENUM_FOR_EACH_X(F, T, startIndex, c), ARROW_ENUM_FOR_EACH_X(F, T, startIndex, d), \
        ARROW_ENUM_FOR_EACH_X(F, T, startIndex, e), ARROW_ENUM_FOR_EACH_X(F, T, startIndex, f)


#define ARROW_ENUM_FOR_EACH_Impl(F, T, startIndex, StepSize) ARROW_ENUM_FOR_EACH_##StepSize(F, T, startIndex)
#define ARROW_ENUM_FOR_EACH(F, T, startIndex, StepSize) ARROW_ENUM_FOR_EACH_Impl(F, T, startIndex, StepSize)

//clang最大只支持128个参数初始化 gcc最大支持256个参数
#define ENUM_STEP_SIZE_16 16
#define ENUM_STEP_SIZE_32 32
#define ENUM_STEP_SIZE_64 64
#define ENUM_STEP_SIZE_128 128
using IntegerSequenceEnumStepSize16 = Arrow::MakeIntegerSequence<ENUM_STEP_SIZE_16>::type;
using IntegerSequenceEnumStepSize32 = Arrow::MakeIntegerSequence<ENUM_STEP_SIZE_32>::type;
using IntegerSequenceEnumStepSize64 = Arrow::MakeIntegerSequence<ENUM_STEP_SIZE_64>::type;
using IntegerSequenceEnumStepSize128 = Arrow::MakeIntegerSequence<ENUM_STEP_SIZE_128>::type;


template<typename T>
struct EnumItemInfo
{
    constexpr EnumItemInfo() : value(0), name(nullptr){}
    constexpr EnumItemInfo(T v, const char* data) : value(v), name(data) {}

    T value;
    const char* name;
};

// 常规执行 [zhuyb 2023-08-19 22:34:31]
template <typename T, typename std::underlying_type<T>::type enumStart, typename std::underlying_type<T>::type enumEnd, typename SFINEA = void>
struct EnumItemList
{
    using __EnumItemInfo = EnumItemInfo<T>;
    using __EnumItemName = details::EnumItemName<T, static_cast<T>(enumStart)>;
    static constexpr Array::ArrayView<__EnumItemInfo, static_cast<size_t>(enumEnd - enumStart + 1)> Array()
    {
        return Array::ArrayView<__EnumItemInfo, 1>(__EnumItemInfo(static_cast<T>(enumStart), __EnumItemName::itemView.data)) + EnumItemList<T, enumStart + 1, enumEnd>::Array();
    }
};

// 递归结束 [zhuyb 2023-08-19 22:34:31]
template <typename T, typename std::underlying_type<T>::type enumStart, typename std::underlying_type<T>::type enumEnd>
struct EnumItemList<T, enumStart, enumEnd, typename std::enable_if<(enumStart == enumEnd)>::type>
{
    using __EnumItemInfo = EnumItemInfo<T>;
    using __EnumItemName = details::EnumItemName<T, static_cast<T>(enumStart)>;
    static constexpr Array::ArrayView<__EnumItemInfo, 1> Array()
    {
        return Array::ArrayView<__EnumItemInfo, 1>(__EnumItemInfo(static_cast<T>(enumStart), __EnumItemName::itemView.data));
    }
};

// 用于提高编译效率。 多倍率执行 [zhuyb 2023-08-19 22:35:25]
#define ENUM_ITEM_LIST_StepSize(StepSize)                                                                                                                                               \
    template <typename T, typename std::underlying_type<T>::type enumStart, typename std::underlying_type<T>::type enumEnd>                                                             \
    struct EnumItemList<T, enumStart, enumEnd, typename std::enable_if<(enumEnd  >= StepSize + enumStart - 1&& enumEnd  < StepSize * 2 + enumStart - 1)>::type>                          \
    {                                                                                                                                                                                   \
        using __EnumItemInfo = EnumItemInfo<T>;                                                                                                                                         \
        static constexpr Array::ArrayView<__EnumItemInfo, static_cast<size_t>(enumEnd - enumStart + 1)> Array()                                                                         \
        {                                                                                                                                                                               \
            return Array::ArrayView<__EnumItemInfo, StepSize>({ARROW_ENUM_FOR_EACH(ENUM_ITEM_INFO, T, enumStart, StepSize)}) + EnumItemList<T, enumStart + StepSize, enumEnd>::Array(); \
        }                                                                                                                                                                               \
    };

//定义16倍执行力
ENUM_ITEM_LIST_StepSize(16)
ENUM_ITEM_LIST_StepSize(32)
ENUM_ITEM_LIST_StepSize(64)
ENUM_ITEM_LIST_StepSize(128)

// template <typename T, typename std::underlying_type<T>::type enumStart, typename std::underlying_type<T>::type enumEnd, typename SFINEA = void>
// struct EnumItemList<T, enumStart, enumEnd, SFINEA, typename std::enable_if<(enumStart <= enumEnd && enumStart + 16 > enumEnd)>::type>
// {
//     constexpr Array::ArrayView<EnumItem> Array()
//     {
//         return Array::ArrayView<EnumItem, 1>(EnumItem<T, static_cast<enumStart>()) + EnumItemList<T, enumStart + 1, enumEnd>::Array()；
//     }
// }


}