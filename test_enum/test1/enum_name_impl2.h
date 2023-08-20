#pragma once
#include "arrow/typelist/static_array.h"
#include "arrow/typelist/static_string.h"
#include "arrow/typelist/type_name/type_name_impl.h"
#include <iostream>
#include <map>
#include <type_traits>





namespace Arrow2
{
using namespace Arrow;

namespace details
{

// 不同平台对应的关键子不一样 [zhuyb 2023-08-13 17:13:06]
#ifdef __clang__
constexpr char NoSocpedEnumNameStart[] = ", val = ";
constexpr char NoSocpedEnumNameEnd[] = ", SFNIEA = ";
// constexpr char NoSocpedEnumInValid[] = "val = (";

constexpr char SocpedEnumNameStart[] = ", val = ";
constexpr char SocpedEnumNameEnd[] = ", SFNIEA = ";
// constexpr char SocpedEnumInValid[] = "val = (";

constexpr char EnumVaild[] = "val = (";
#elif __GNUC__
constexpr char NoSocpedEnumNameStart[] = "EnumType val = ";
constexpr char NoSocpedEnumNameEnd[] = ";";
constexpr char NoSocpedEnumInValid[] = "val = (";

constexpr char SocpedEnumNameStart[] = "::";
constexpr char SocpedEnumNameEnd[] = ";";
constexpr char SocpedEnumInValid[] = "val = (";
#else
#endif

// 判断枚举是 class enum 还是 enum
template <typename EnumType, typename = typename std::enable_if<std::is_enum<EnumType >::value>::type>
struct SocpedEnum : std::false_type
{
};

template <typename EnumType>
struct SocpedEnum<EnumType , typename std::enable_if<!std::is_convertible<EnumType , int>::value>::type> : std::true_type
{
};

// 调试使用 [zhuyb 2023-08-13 17:13:49]
template <typename EnumType, EnumType val>
size_t Trace()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    return 0;
}

// template <typename EnumType, EnumType val>
// struct Valid : public std::false_type{};

// 判断枚举值是否合法 [zhuyb 2023-08-13 17:13:55]
template <typename EnumType, EnumType val>
constexpr bool IsValid()
{
    return StaticStr::Find<1>(__PRETTY_FUNCTION__, details::EnumVaild) == std::string::npos;
}

// 枚举合法性定义 [zhuyb 2023-08-20 18:26:22]
template <typename EnumType, EnumType val, typename SFINEA = void>
struct Valid : public std::false_type
{
};

template <typename EnumType, EnumType val>
struct Valid<EnumType , val, typename std::enable_if<IsValid<EnumType , val>()>::type> : public std::true_type
{
};

// 具体枚举的 [zhuyb 2023-08-20 18:26:52]
template <typename EnumType, EnumType val, typename SFNIEA = typename details::Valid<EnumType , val>::type>
struct EnumItemStrView
{
};

template <typename EnumType, EnumType val>
struct EnumItemStrView<EnumType , val, std::true_type>
{

    // 获取限定性枚举 名称的长度 gcc 在处理size_t会自动添加模板参数列表中添加 size_t = long unsigned int  [zhuyb 2023-08-13 17:14:29]
    // gcc 输出格式：size_t Arrow2::LengthScopedEnum() [with EnumType = Enum...; EnumType t = Enum...; size_t = long unsigned int]
    // clang 输出格式：size_t Arrow2::LengthScopedEnum() [with EnumType = Enum...; EnumType t = Enum...; size_t = long unsigned int]
    static constexpr size_t LengthScopedEnum()
    {
        return StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameEnd) - StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameStart) - StaticStr::StrLen(SocpedEnumNameStart);
    }

    // 测试使用 [zhuyb 2023-08-14 13:42:33]
    static size_t TraceLengthScopedEnum()
    {
        std::cout << "ScopedEnum" << std::endl;
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::cout << "EndStr:\"" << SocpedEnumNameEnd << "\""
                  << " End Last Index:" << StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameEnd) << std::endl;
        std::cout << "StartStr:\"" << SocpedEnumNameStart << "\""
                  << "Start Last Index:" << StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameStart) << std::endl;
        std::cout << "StartCharLen::" << StaticStr::StrLen(SocpedEnumNameStart) << std::endl;
        std::cout << "Lenth1:" << StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameEnd) - StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameStart) - StaticStr::StrLen(SocpedEnumNameStart) << std::endl;
        // std::cout << "Length2:" << LengthScopedEnum() << std::endl;
        return 0;
    }

    // 获取非限定枚举 名称的长度 [zhuyb 2023-08-13 17:30:38]
    static constexpr size_t LengthNoScopedEnum()
    {
        return StaticStr::Find<0>(__PRETTY_FUNCTION__, NoSocpedEnumNameEnd) - StaticStr::Find<1>(__PRETTY_FUNCTION__, NoSocpedEnumNameStart) - StaticStr::StrLen(NoSocpedEnumNameStart);
    }

    // 测试使用 [zhuyb 2023-08-14 15:19:20]
    static size_t TraceLengthNoScopedEnum()
    {
        std::cout << "NoScopedEnum" << std::endl;
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::cout << "EndStr:\"" << NoSocpedEnumNameEnd << "\""
                  << " End Last Index:" << StaticStr::Find<0>(__PRETTY_FUNCTION__, NoSocpedEnumNameEnd) << std::endl;
        std::cout << "StartStr:\"" << NoSocpedEnumNameStart << "\""
                  << "Start First Index:" << StaticStr::Find<1>(__PRETTY_FUNCTION__, NoSocpedEnumNameStart) << std::endl;
        std::cout << "StartCharLen:" << StaticStr::StrLen(NoSocpedEnumNameStart) << std::endl;
        std::cout << "Length1:" << StaticStr::Find<0>(__PRETTY_FUNCTION__, NoSocpedEnumNameEnd) - StaticStr::Find<1>(__PRETTY_FUNCTION__, NoSocpedEnumNameStart) - StaticStr::StrLen(NoSocpedEnumNameStart) << std::endl;
        std::cout << "Length2:" << LengthNoScopedEnum() << std::endl;
        return 0;
    }

    // 获取长度 [zhuyb 2023-08-13 17:31:04]
    static constexpr size_t Length()
    {
        return SocpedEnum<EnumType >::value ? LengthScopedEnum() : LengthNoScopedEnum();
    }

    // 获取长度 [zhuyb 2023-08-13 17:31:04]
    static void TraceLength()
    {
        SocpedEnum<EnumType >::value ? TraceLengthScopedEnum() : TraceLengthNoScopedEnum();
    }

    // 获取非限定枚举的名称 [zhuyb 2023-08-13 17:31:10]
    static constexpr auto NoSpcpedEnumItemName() -> StaticStr::StringView<Length()>
    {
        return StaticStr::StringView<Length()>(__PRETTY_FUNCTION__ + StaticStr::Find<1>(__PRETTY_FUNCTION__, NoSocpedEnumNameStart) + StaticStr::StrLen(NoSocpedEnumNameStart));
    }

    // 获取限定枚举的名称 [zhuyb 2023-08-13 17:31:28]
    static constexpr auto SocpedEnumItemName() -> StaticStr::StringView<Length()>
    {
        return StaticStr::StringView<Length()>(__PRETTY_FUNCTION__ + StaticStr::Find<0>(__PRETTY_FUNCTION__, SocpedEnumNameStart) + StaticStr::StrLen(SocpedEnumNameStart));
    }

    static constexpr auto Impl() -> StaticStr::StringView<Length()>
    {
        return SocpedEnum<EnumType >::value ? SocpedEnumItemName() : NoSpcpedEnumItemName();
    }

    static constexpr decltype(Impl()) value = Impl();
};
template <typename EnumType, EnumType val>
constexpr decltype(EnumItemStrView<EnumType , val, std::true_type>::Impl()) EnumItemStrView<EnumType , val, std::true_type>::value;

template <typename EnumType, EnumType val>
struct EnumItemStrView<EnumType , val, std::false_type>
{
    using valueType = typename std::underlying_type<EnumType >::type;
    static constexpr auto Impl() -> decltype(Arrow::TypeName<EnumType >::Impl() + StaticStr::Str(" undefined:") + StaticStr::NumToStr<valueType, static_cast<valueType>(val)>())
    {
        return Arrow::TypeName<EnumType >::Impl() + StaticStr::Str(" undefined:") + StaticStr::NumToStr<valueType, static_cast<valueType>(val)>();
    }

    static constexpr decltype(EnumItemStrView<EnumType , val, std::false_type>::Impl()) value = EnumItemStrView<EnumType , val, std::false_type>::Impl();
};
template <typename EnumType, EnumType val>
constexpr decltype(EnumItemStrView<EnumType , val, std::false_type>::Impl()) EnumItemStrView<EnumType , val, std::false_type>::value;
}

template<typename EnumType>
struct EnumItemInfo
{
    constexpr EnumItemInfo() : value(0), name(nullptr){}
    constexpr EnumItemInfo(EnumType v, const char* data) : value(v), name(data) {}

    EnumType value;
    const char* name;
};


#define IS_VALID(T, startIndex, i, j) Arrow2::details::IsValid<EnumType , static_cast<EnumType >(startIndex + 0x##i##j)>()

#define ENUM_ITEM_NAME(T, startIndex, i, j) Arrow2::details::EnumItemStrView<EnumType , static_cast<EnumType >(startIndex + 0x##i##j)>::value.data

#define ENUM_ITEM_INFO(T, startIndex, i, j) Arrow2::EnumItemInfo<EnumType >(static_cast<EnumType >(startIndex + 0x##i##j), ENUM_ITEM_NAME(T, startIndex, i, j))

#define ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, i, j) F(T, startIndex, i,j)

#define ARROW_ENUM_FOR_EACH_I(F, T, startIndex, i) ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, i, 0), \
                                                   ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, i, 1), \
                                                   ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, i, 2), \
                                                   ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, i, 3), \
                                                   ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, i, 4), \
                                                   ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, i, 5), \
                                                   ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, i, 6), \
                                                   ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, i, 7), \
                                                   ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, i, 8), \
                                                   ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, i, 9), \
                                                   ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, i, a), \
                                                   ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, i, b), \
                                                   ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, i, c), \
                                                   ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, i, d), \
                                                   ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, i, e), \
                                                   ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, i, f)

#define ARROW_ENUM_FOR_EACH_2(F, T, startIndex) ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, 0, 0), \
                                                ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, 0, 1)

#define ARROW_ENUM_FOR_EACH_4(F, T, startIndex) ARROW_ENUM_FOR_EACH_2(F, T, startIndex),         \
                                                ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, 0, 2), \
                                                ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, 0, 3)

#define ARROW_ENUM_FOR_EACH_8(F, T, startIndex) ARROW_ENUM_FOR_EACH_4(F, T, startIndex),         \
                                                ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, 0, 4), \
                                                ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, 0, 5), \
                                                ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, 0, 6), \
                                                ARROW_ENUM_FOR_EACH_I_J(F, T, startIndex, 0, 7)

#define ARROW_ENUM_FOR_EACH_16(F, T, startIndex) ARROW_ENUM_FOR_EACH_I(F, T, startIndex, 0)

#define ARROW_ENUM_FOR_EACH_32(F, T, startIndex) ARROW_ENUM_FOR_EACH_I(F, T, startIndex, 0), \
                                                 ARROW_ENUM_FOR_EACH_I(F, T, startIndex, 1)

#define ARROW_ENUM_FOR_EACH_64(F, T, startIndex) ARROW_ENUM_FOR_EACH_32(F, T, startIndex),   \
                                                 ARROW_ENUM_FOR_EACH_I(F, T, startIndex, 2), \
                                                 ARROW_ENUM_FOR_EACH_I(F, T, startIndex, 3)

#define ARROW_ENUM_FOR_EACH_128(F, T, startIndex) ARROW_ENUM_FOR_EACH_64(F, T, startIndex),   \
                                                  ARROW_ENUM_FOR_EACH_I(F, T, startIndex, 4), \
                                                  ARROW_ENUM_FOR_EACH_I(F, T, startIndex, 5), \
                                                  ARROW_ENUM_FOR_EACH_I(F, T, startIndex, 6), \
                                                  ARROW_ENUM_FOR_EACH_I(F, T, startIndex, 7)

#define ARROW_ENUM_FOR_EACH_256(F, T, startIndex) ARROW_ENUM_FOR_EACH_128(F, T, startIndex),  \
                                                  ARROW_ENUM_FOR_EACH_I(F, T, startIndex, 8), \
                                                  ARROW_ENUM_FOR_EACH_I(F, T, startIndex, 9), \
                                                  ARROW_ENUM_FOR_EACH_I(F, T, startIndex, a), \
                                                  ARROW_ENUM_FOR_EACH_I(F, T, startIndex, b), \
                                                  ARROW_ENUM_FOR_EACH_I(F, T, startIndex, c), \
                                                  ARROW_ENUM_FOR_EACH_I(F, T, startIndex, d), \
                                                  ARROW_ENUM_FOR_EACH_I(F, T, startIndex, e), \
                                                  ARROW_ENUM_FOR_EACH_I(F, T, startIndex, f)

#define ARROW_ENUM_FOR_EACH_Impl(F, T, startIndex, StepSize) ARROW_ENUM_FOR_EACH_##StepSize(F, T, startIndex)
#define ARROW_ENUM_FOR_EACH(F, T, startIndex, StepSize) ARROW_ENUM_FOR_EACH_Impl(F, T, startIndex, StepSize)

// // clang最大只支持128个参数初始化 gcc最大支持256个参数
// #define ENUM_STEP_SIZE_16 16
// #define ENUM_STEP_SIZE_32 32
// #define ENUM_STEP_SIZE_64 64
// #define ENUM_STEP_SIZE_128 128
// using IntegerSequenceEnumStepSize16 = Arrow::MakeIntegerSequence<ENUM_STEP_SIZE_16>::type;
// using IntegerSequenceEnumStepSize32 = Arrow::MakeIntegerSequence<ENUM_STEP_SIZE_32>::type;
// using IntegerSequenceEnumStepSize64 = Arrow::MakeIntegerSequence<ENUM_STEP_SIZE_64>::type;
// using IntegerSequenceEnumStepSize128 = Arrow::MakeIntegerSequence<ENUM_STEP_SIZE_128>::type;


// 常规执行 [zhuyb 2023-08-19 22:34:31]
template <typename EnumType, typename std::underlying_type<EnumType >::type enumStart, typename std::underlying_type<EnumType >::type enumEnd, typename SFINEA = void>
struct EnumItemArrayInterval
{
    using __EnumItemInfo = EnumItemInfo<EnumType >;
    using __EnumItemStrView = details::EnumItemStrView<EnumType , static_cast<EnumType >(enumStart)>;
    static constexpr Array::ArrayView<__EnumItemInfo, static_cast<size_t>(enumEnd - enumStart + 1)> Array()
    {
        return Array::ArrayView<__EnumItemInfo, 1>(__EnumItemInfo(static_cast<EnumType >(enumStart), __EnumItemStrView::value.data)) + 
                EnumItemArrayInterval<EnumType , enumStart + 1, enumEnd>::Array();
    }
};

// 递归结束 [zhuyb 2023-08-19 22:34:31]
template <typename EnumType, typename std::underlying_type<EnumType >::type enumStart, typename std::underlying_type<EnumType >::type enumEnd>
struct EnumItemArrayInterval<EnumType , enumStart, enumEnd, typename std::enable_if<(enumStart == enumEnd)>::type>
{
    using __EnumItemInfo = EnumItemInfo<EnumType >;
    using __EnumItemStrView = details::EnumItemStrView<EnumType , static_cast<EnumType >(enumStart)>;
    static constexpr Array::ArrayView<__EnumItemInfo, 1> Array()
    {
        return Array::ArrayView<__EnumItemInfo, 1>(__EnumItemInfo(static_cast<EnumType >(enumStart), __EnumItemStrView::value.data));
    }
};

// 用于提高编译效率。 多倍率执行 [zhuyb 2023-08-19 22:35:25]
#define ENUM_ITEM_LIST_StepSize(StepSize)                                                                                                                                                               \
    template <typename EnumType, typename std::underlying_type<EnumType>::type enumStart, typename std::underlying_type<EnumType>::type enumEnd>                                                        \
    struct EnumItemArrayInterval<EnumType, enumStart, enumEnd, typename std::enable_if<(enumEnd >= StepSize + enumStart - 1 && enumEnd < StepSize * 2 + enumStart - 1)>::type>                          \
    {                                                                                                                                                                                                   \
        using __EnumItemInfo = EnumItemInfo<EnumType>;                                                                                                                                                  \
        static constexpr Array::ArrayView<__EnumItemInfo, static_cast<size_t>(enumEnd - enumStart + 1)> Array()                                                                                         \
        {                                                                                                                                                                                               \
            return Array::ArrayView<__EnumItemInfo, StepSize>({ARROW_ENUM_FOR_EACH(ENUM_ITEM_INFO, T, enumStart, StepSize)}) + EnumItemArrayInterval<EnumType, enumStart + StepSize, enumEnd>::Array(); \
        }                                                                                                                                                                                               \
    };

//定义16倍执行力
ENUM_ITEM_LIST_StepSize(4)
ENUM_ITEM_LIST_StepSize(8)
ENUM_ITEM_LIST_StepSize(16)
ENUM_ITEM_LIST_StepSize(32)
ENUM_ITEM_LIST_StepSize(64)
ENUM_ITEM_LIST_StepSize(128)

template <typename EnumType, EnumType... enumArgs>
struct EnumItemArray{};

template <typename EnumType, EnumType enumStart1, EnumType enumEnd1, EnumType enumStart2, EnumType... enumArgs>
struct EnumItemArray<EnumType, enumStart1, enumEnd1, enumStart2, enumArgs...>
{

    static_assert(sizeof...(enumArgs) % 2 == 1 , "枚举参数的个数必须为偶数个，首尾成对出现");
    static_assert(enumEnd1 >= enumStart1 ,"枚举值必须连续,且为递增序列");
    static_assert(enumEnd1 <= enumStart2 ,"枚举值必须连续,且为递增序列");
    // using type = details::EnumListToStrImpl<EnumType, enumArgs...>;
    // using value = details::EnumListData<EnumType>;

    // // static bool gInit;
    // static const char* ItemStr(EnumType enumValue)
    // {
    //     // static bool bInit = type::Init();
    //     auto it = value::value.find(enumValue);
    //     if(it == value::value.end())
    //     {
    //         return "";
    //     }
    //     return it->second;
    // }
};

}