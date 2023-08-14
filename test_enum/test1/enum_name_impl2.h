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

template<typename T, T l, T r, typename = typename std::enable_if<(l < r)>::type>
struct less : std::true_type{};

template<typename T, T l, T r>
struct less <T, l, r, typename std::enable_if<(l >= r)>::type> : std::true_type{};


// 判断枚举值是否合法 [zhuyb 2023-08-13 17:13:55]
template<typename T, T t>
constexpr bool IsValid()
{
    return StaticStr::Find<0>(__PRETTY_FUNCTION__, details::NoSocpedEnumInValid) == std::string::npos;
}

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
    std::cout << StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameEnd) << std::endl;
    std::cout << StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameStart) << std::endl;
    std::cout << StaticStr::StrLen(SocpedEnumNameStart) << std::endl;
    std::cout << StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameEnd) - StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameStart) - StaticStr::StrLen(SocpedEnumNameStart)<< std::endl;
    std::cout << LengthScopedEnum<T, t>() << std::endl;
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
    std::cout << StaticStr::Find<0>(__PRETTY_FUNCTION__, NoSocpedEnumNameEnd) << std::endl;
    std::cout << StaticStr::Find<1>(__PRETTY_FUNCTION__, NoSocpedEnumNameStart) << std::endl;
    std::cout << StaticStr::StrLen(NoSocpedEnumNameStart) << std::endl;
    std::cout << StaticStr::Find<0>(__PRETTY_FUNCTION__, NoSocpedEnumNameEnd) - StaticStr::Find<1>(__PRETTY_FUNCTION__, NoSocpedEnumNameStart) - StaticStr::StrLen(NoSocpedEnumNameStart) << std::endl;
    std::cout << LengthNoScopedEnum<T, t>() << std::endl;
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
size_t Trace()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return 0;
}

// 获取枚举短名称(不包括限定域名称) [zhuyb 2023-08-14 15:19:38]
template<typename T, T t>
constexpr auto EnumItemName() -> StaticStr::StringView<details::Length<T, t>()>
{
    return  details::IsSocpedEnum<T>::value ? details::SocpedEnumItemName<T, t>() : details::NoSpcpedEnumItemName<T, t>();
}

#define ARROW_ENUM_FOR_EACH_1(T, startIndex, i, j) details::IsValid<T, static_cast<T>(startIndex + 0x##i##j)>()

#define ARROW_ENUM_FOR_EACH_16(T, startIndex, i)                                                \
    ARROW_ENUM_FOR_EACH_1(T, startIndex, i, 0), ARROW_ENUM_FOR_EACH_1(T, startIndex, i, 1),        \
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

// // 探测对应的数值哪些位置有枚举定义 [zhuyb 2023-08-14 15:20:36]
// template<typename T, typename std::underlying_type<T>::type enumStart>
// constexpr Array::ArrayView<bool, EnumStepSize> ValidCount()
// {
//     return Array::ArrayView<bool, EnumStepSize>({ARROW_ENUM_FOR_EACH(T, enumStart, EnumStepSize)});
// }

// 计算有效枚举个数 [zhuyb 2023-08-14 18:00:55]
template<typename T>
struct ValidSize;

template<int ...args>
struct ValidSize<Arrow::IntegerSequence<args...>>
{
    constexpr static size_t Impl(const bool* array)
    {
        return (... + (array[args] ? 1 : 0));
    }
};

//  [zhuyb 2023-08-14 18:01:34]
template <typename T, typename std::underlying_type<T>::type enumStart, size_t index,  size_t length>
struct ValidCountConvert
{
    constexpr static Array::ArrayView<T, length> Impl(const bool* array)
    {
        return array[index] ? Array::ArrayView<T, 1>(static_cast<T>(enumStart + index)) + ValidCountConvert<T, enumStart, index + 1, length - 1>::Impl(array) : ValidCountConvert<T, enumStart, index + 1, length>::Impl(array);
    }
};

template <typename T, typename std::underlying_type<T>::type enumStart, size_t length>
struct ValidCountConvert<T, enumStart, EnumStepSize, length>
{
    constexpr static Array::ArrayView<T, length> Impl(const bool* array)
    {
        return Array::ArrayView<T, length>();
    }
};

template <typename T, typename std::underlying_type<T>::type enumStart,  size_t index>
struct ValidCountConvert<T, enumStart, index, 0>
{
    constexpr static Array::ArrayView<T, 0> Impl(const bool* array)
    {
        return Array::ArrayView<T, 0>();
    }
};

template <typename T, typename std::underlying_type<T>::type enumStart>
struct ValidCountConvert<T, enumStart, EnumStepSize, 0>
{
    constexpr static Array::ArrayView<T, 0> Impl(const bool* array)
    {
        return Array::ArrayView<T, 0>();
    }
};


// 临时变量，找出enumStart ~ enumStart+EnumStepSize 有效的枚举 [zhuyb 2023-08-14 18:02:03]
template<typename T, typename std::underlying_type<T>::type enumStart, typename std::underlying_type<T>::type enumEnd, typename SFINAE = void>
struct ValidCountTmp
{
    constexpr static size_t length = (EnumStepSize + enumEnd - enumStart) / EnumStepSize * EnumStepSize;

    constexpr static Array::ArrayView<bool, length> Impl()
    {
        return Array::ArrayView<bool, EnumStepSize>({ARROW_ENUM_FOR_EACH(T, enumStart, EnumStepSize)}) + ValidCountTmp<T, enumStart + EnumStepSize, enumEnd>::Impl();
    }
};

template<typename T, typename std::underlying_type<T>::type enumStart, typename std::underlying_type<T>::type enumEnd>
struct ValidCountTmp<T, enumStart, enumEnd, typename std::enable_if<(enumStart >= enumEnd)>::type>
{
    constexpr static size_t length = 0;
    constexpr static Array::ArrayView<bool, length> Impl()
    {
        return Array::ArrayView<bool, 0>{};
    }
};

// 探测对应的数值哪些位置有枚举定义 [zhuyb 2023-08-14 15:20:36]
template<typename T, typename std::underlying_type<T>::type enumStart, typename std::underlying_type<T>::type enumEnd>
constexpr auto MyEnumName1() -> decltype(ValidCountTmp<T, enumStart, enumEnd>::Impl())
{
    return ValidCountTmp<T, enumStart, enumEnd>::Impl();
}

// 探测对应的数值哪些位置有枚举定义 [zhuyb 2023-08-14 15:20:36]
template<typename T, typename std::underlying_type<T>::type enumStart, typename std::underlying_type<T>::type enumEnd>
constexpr size_t MyEnumName2()
{
    return Arrow2::ValidSize<Arrow2::IntegerSequence_EnumStepSize>::Impl(MyEnumName1<T, enumStart, enumEnd>().data);
}

}