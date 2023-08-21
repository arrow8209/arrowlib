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


// 判断枚举是 class enum 还是 enum
template <typename EnumType, typename = typename std::enable_if<std::is_enum<EnumType >::value>::type>
struct IsScopedEnum : std::false_type
{
};

template <typename EnumType>
struct IsScopedEnum<EnumType , typename std::enable_if<!std::is_convertible<EnumType , int>::value>::type> : std::true_type
{
};

// 判断枚举值是否合法 ture 合法 false 不合法 [zhuyb 2023-08-13 17:13:55]
template <typename EnumType, EnumType val>
constexpr bool IsValid()
{
    return StaticStr::Find<1>(__PRETTY_FUNCTION__, "val = (") == std::string::npos;
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

// 获取枚举的字符串 [zhuyb 2023-08-21 15:46:23]
template <typename T, T val, typename EndTag=void>
struct EnumItemStrViewTmp
{
    static size_t TraceLength()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return 0;
    }
    static constexpr size_t Length()
    {
        return StaticStr::Find<1>(__PRETTY_FUNCTION__, " EndTag = ") - 1 - StaticStr::Find<1>(__PRETTY_FUNCTION__, " val = ") - StaticStr::StrLen(" val = ");
    }

    static constexpr auto View() -> StaticStr::StringView<Length()>
    {   
        return StaticStr::StringView<Length()>(__PRETTY_FUNCTION__ + StaticStr::Find<1>(__PRETTY_FUNCTION__, " val = ") + StaticStr::StrLen(" val = "));
    }
    static constexpr decltype(EnumItemStrViewTmp<T, val, EndTag>::View()) value = EnumItemStrViewTmp<T, val, EndTag>::View();
    static constexpr size_t length = Length();
};
template <typename T, T val, typename EndTag>
constexpr decltype(EnumItemStrViewTmp<T, val, EndTag>::View()) EnumItemStrViewTmp<T, val, EndTag>::value;
template<typename T, T val, typename EndTag>
constexpr size_t EnumItemStrViewTmp<T, val, EndTag>::length;


// 结构体定义 [zhuyb 2023-08-21 15:42:48]
template <typename EnumType, EnumType val,
          typename Valid = typename Valid<EnumType, val>::type,
          typename ScopedEnum = typename IsScopedEnum<EnumType>::type>
struct EnumItemStrView
{
};

// 限定枚举类型 [zhuyb 2023-08-21 15:42:36]
template <typename EnumType, EnumType val>
struct EnumItemStrView<EnumType , val, std::true_type, std::true_type>
{
    using __EnumItemStrViewTmp = EnumItemStrViewTmp<EnumType, val>;

    constexpr static size_t Length()
    {
        return __EnumItemStrViewTmp::length - StaticStr::StrLen("::") - StaticStr::Find<1>(__EnumItemStrViewTmp::value, "::");
    }

    // 获取限定枚举的名称 [zhuyb 2023-08-13 17:31:28]
    static constexpr auto Str() -> StaticStr::StringView<Length()>
    {
        return StaticStr::SubStr<__EnumItemStrViewTmp::length - Length()>(__EnumItemStrViewTmp::value);
    }

    static constexpr decltype(EnumItemStrView<EnumType, val, std::true_type, std::true_type>::Str()) strView = 
                EnumItemStrView<EnumType, val, std::true_type, std::true_type>::Str();
    static constexpr const char* str =strView.data;
    static constexpr EnumType value=val;
};
template <typename EnumType, EnumType val>
constexpr decltype(EnumItemStrView<EnumType, val, std::true_type, std::true_type>::Str())
    EnumItemStrView<EnumType, val, std::true_type, std::true_type>::strView;
template <typename EnumType, EnumType val>
constexpr const char* EnumItemStrView<EnumType, val, std::true_type, std::true_type>::str;
template <typename EnumType, EnumType val>
constexpr EnumType EnumItemStrView<EnumType, val, std::true_type, std::true_type>::value;

// 非限定枚举类型 [zhuyb 2023-08-21 15:42:15]
template <typename EnumType, EnumType val>
struct EnumItemStrView<EnumType , val, std::true_type, std::false_type>
{
    using __EnumItemStrViewTmp = EnumItemStrViewTmp<EnumType, val>;

    static constexpr decltype(__EnumItemStrViewTmp::value) strView = __EnumItemStrViewTmp::value;
    static constexpr const char* str =strView.data;
    static constexpr EnumType value=val;
};
template <typename EnumType, EnumType val>
constexpr decltype(EnumItemStrViewTmp<EnumType, val>::value) EnumItemStrView<EnumType, val, std::true_type, std::false_type>::strView;
template <typename EnumType, EnumType val>
constexpr const char* EnumItemStrView<EnumType, val, std::true_type, std::false_type>::str;
template <typename EnumType, EnumType val>
constexpr EnumType EnumItemStrView<EnumType, val, std::true_type, std::false_type>::value;

// 不合法的枚举类型 [zhuyb 2023-08-21 15:43:01]
template <typename EnumType, EnumType val, typename ScopedEnum>
struct EnumItemStrView<EnumType , val, std::false_type, ScopedEnum>
{
    using valueType = typename std::underlying_type<EnumType >::type;
    static constexpr auto Str() -> decltype(Arrow::TypeName<EnumType >::Impl() + StaticStr::Str(" undefined:") + StaticStr::NumToStr<valueType, static_cast<valueType>(val)>())
    {
        return Arrow::TypeName<EnumType >::Impl() + StaticStr::Str(" undefined:") + StaticStr::NumToStr<valueType, static_cast<valueType>(val)>();
    }

    static constexpr decltype(EnumItemStrView<EnumType, val, std::false_type, ScopedEnum>::Str()) strView = EnumItemStrView<EnumType, val, std::false_type, ScopedEnum>::Str();
    static constexpr const char* str = strView.data;
    static constexpr EnumType value = val;
};
template <typename EnumType, EnumType val, typename ScopedEnum>
constexpr decltype(EnumItemStrView<EnumType, val, std::false_type, ScopedEnum>::Str())
    EnumItemStrView<EnumType, val, std::false_type, ScopedEnum>::strView;
    template <typename EnumType, EnumType val, typename ScopedEnum>
constexpr const char* EnumItemStrView<EnumType, val, std::false_type, ScopedEnum>::str;
template <typename EnumType, EnumType val, typename ScopedEnum>
constexpr EnumType EnumItemStrView<EnumType, val, std::false_type, ScopedEnum>::value;


template<typename EnumType>
struct EnumItemInfo
{
    constexpr EnumItemInfo() : value(static_cast<EnumType>(0)), str(nullptr){}
    constexpr EnumItemInfo(EnumType v, const char* data) : value(v), str(data) {}

    EnumType value;
    const char* str;
};


#define IS_VALID(T, startIndex, i, j) IsValid<T, static_cast<T>(startIndex + 0x##i##j)>()

#define ENUM_ITEM_NAME(T, startIndex, i, j) EnumItemStrView<T , static_cast<T>(startIndex + 0x##i##j)>::str

#define ENUM_ITEM_INFO(T, startIndex, i, j) EnumItemInfo<T>(static_cast<T>(startIndex + 0x##i##j), ENUM_ITEM_NAME(T, startIndex, i, j))

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
template <typename EnumType,
          typename std::underlying_type<EnumType>::type enumStart,
          typename std::underlying_type<EnumType>::type enumEnd,
          typename SFINEA = void>
struct EnumItemArrayInterval
{
    using __EnumItemInfo = EnumItemInfo<EnumType>;
    using __EnumItemStrView = EnumItemStrView<EnumType, static_cast<EnumType>(enumStart)>;
    static constexpr auto Array() -> decltype(Array::ArrayView<__EnumItemInfo, 1>() + EnumItemArrayInterval<EnumType , enumStart + 1, enumEnd>::Array())
    {
        return Array::ArrayView<__EnumItemInfo, 1>(__EnumItemInfo(static_cast<EnumType >(enumStart), __EnumItemStrView::str)) + 
                EnumItemArrayInterval<EnumType , enumStart + 1, enumEnd>::Array();
    }
};

// 递归结束 [zhuyb 2023-08-19 22:34:31]
template <typename EnumType, typename std::underlying_type<EnumType >::type enumStart, typename std::underlying_type<EnumType >::type enumEnd>
struct EnumItemArrayInterval<EnumType , enumStart, enumEnd, typename std::enable_if<(enumStart > enumEnd)>::type>
{
    using __EnumItemInfo = EnumItemInfo<EnumType >;
    static constexpr Array::ArrayView<__EnumItemInfo, 0> Array()
    {
        return Array::ArrayView<__EnumItemInfo, 0>();
    }
};

// 用于提高编译效率。 多执行 [zhuyb 2023-08-19 22:35:25]
#define ENUM_ITEM_LIST_StepSize(StepSize)                                                                                                                                                                      \
    template <typename EnumType, typename std::underlying_type<EnumType>::type enumStart, typename std::underlying_type<EnumType>::type enumEnd>                                                               \
    struct EnumItemArrayInterval<EnumType, enumStart, enumEnd, typename std::enable_if<(enumEnd >= StepSize + enumStart - 1 && enumEnd < StepSize * 2 + enumStart - 1)>::type>                                 \
    {                                                                                                                                                                                                          \
        using __EnumItemInfo = EnumItemInfo<EnumType>;                                                                                                                                                         \
        static constexpr auto Array() -> decltype(Array::ArrayView<__EnumItemInfo, StepSize>() + EnumItemArrayInterval<EnumType, enumStart + StepSize, enumEnd>::Array())                                      \
        {                                                                                                                                                                                                      \
            return Array::ArrayView<__EnumItemInfo, StepSize>({ARROW_ENUM_FOR_EACH(ENUM_ITEM_INFO, EnumType, enumStart, StepSize)}) + EnumItemArrayInterval<EnumType, enumStart + StepSize, enumEnd>::Array(); \
        }                                                                                                                                                                                                      \
    };

//定义16倍执行力
ENUM_ITEM_LIST_StepSize(4)
ENUM_ITEM_LIST_StepSize(8)
ENUM_ITEM_LIST_StepSize(16)
ENUM_ITEM_LIST_StepSize(32)
ENUM_ITEM_LIST_StepSize(64)

#define MAX_STEP_SIZE 128
template <typename EnumType, typename std::underlying_type<EnumType>::type enumStart, typename std::underlying_type<EnumType>::type enumEnd>                                                               
struct EnumItemArrayInterval<EnumType, enumStart, enumEnd, typename std::enable_if<(enumEnd >= MAX_STEP_SIZE + enumStart - 1 )>::type>                                 
{                                                                                                                                                                                                          
    using __EnumItemInfo = EnumItemInfo<EnumType>;                                                                                                                                                         
    static constexpr auto Array() -> decltype(Array::ArrayView<__EnumItemInfo, MAX_STEP_SIZE>() + EnumItemArrayInterval<EnumType, enumStart + MAX_STEP_SIZE, enumEnd>::Array())                                      
    {                                                                                                                                                                                                      
        return Array::ArrayView<__EnumItemInfo, MAX_STEP_SIZE>({ARROW_ENUM_FOR_EACH(ENUM_ITEM_INFO, EnumType, enumStart, MAX_STEP_SIZE)}) + EnumItemArrayInterval<EnumType, enumStart + MAX_STEP_SIZE, enumEnd>::Array(); 
    }                                                                                                                                                                                                      
};
}


template <typename EnumType, EnumType... enumArgs>
struct EnumItemArray{};

// 一个参数 [zhuyb 2023-08-21 21:06:35]
template <typename EnumType, EnumType enumStart>
struct EnumItemArray<EnumType, enumStart>
{
    using __DataType = typename std::underlying_type<EnumType>::type;
    using __EnumItemInfo = details::EnumItemInfo<EnumType>;                                                                                                                                                         
    constexpr static auto Array() -> Array::ArrayView<__EnumItemInfo, MAX_STEP_SIZE>
    {
        return details::EnumItemArrayInterval<EnumType, static_cast<__DataType>(enumStart), static_cast<__DataType>(enumStart) + MAX_STEP_SIZE - 1>::Array();
    }

    constexpr static decltype(Array()) array = Array();
    constexpr static size_t length = array.length;
};
template <typename EnumType, EnumType enumStart>
constexpr decltype(EnumItemArray<EnumType, enumStart>::Array()) EnumItemArray<EnumType, enumStart>::array;

// 二个参数 [zhuyb 2023-08-21 21:06:35]
template <typename EnumType, EnumType enumStart, EnumType enumEnd>
struct EnumItemArray<EnumType, enumStart, enumEnd>
{
    using __DataType = typename std::underlying_type<EnumType>::type;
    using __EnumItemInfo = details::EnumItemInfo<EnumType>;                                                                                                                                                         
    constexpr static auto Array() -> decltype(details::EnumItemArrayInterval<EnumType, static_cast<__DataType>(enumStart), static_cast<__DataType>(enumEnd)>::Array())
    {
        return details::EnumItemArrayInterval<EnumType, static_cast<__DataType>(enumStart), static_cast<__DataType>(enumEnd)>::Array();
    }

    constexpr static decltype(Array()) array = Array();
    constexpr static size_t length = array.length;

};
template <typename EnumType, EnumType enumStart, EnumType enumEnd>
constexpr decltype(EnumItemArray<EnumType, enumStart, enumEnd>::Array()) EnumItemArray<EnumType, enumStart, enumEnd>::array;

template <typename EnumType, EnumType enumStart1, EnumType enumEnd1, EnumType enumStart2, EnumType... enumArgs>
struct EnumItemArray<EnumType, enumStart1, enumEnd1, enumStart2, enumArgs...>
{

    static_assert(sizeof...(enumArgs) % 2 == 1 , "枚举参数的个数必须为偶数个，首尾成对出现");
    static_assert(enumStart1 <= enumEnd1 && enumEnd1 < enumStart2 ,"枚举值必须连续,且为递增序列");

    using __DataType = typename std::underlying_type<EnumType>::type;
    using __EnumItemInfo = details::EnumItemInfo<EnumType>; 

    constexpr static auto Array() -> decltype(EnumItemArray<EnumType, enumStart1, enumEnd1>::Array() + EnumItemArray<EnumType, enumStart2, enumArgs...>::Array())
    {
        return EnumItemArray<EnumType, enumStart1, enumEnd1>::Array() + EnumItemArray<EnumType, enumStart2, enumArgs...>::Array();
    }
    constexpr static decltype(Array()) array = Array();
    constexpr static size_t length = array.length;

};
template <typename EnumType, EnumType enumStart1, EnumType enumEnd1, EnumType enumStart2, EnumType... enumArgs>
constexpr decltype(EnumItemArray<EnumType, enumStart1, enumEnd1, enumStart2, enumArgs...>::Array()) EnumItemArray<EnumType, enumStart1, enumEnd1, enumStart2, enumArgs...>::array;

template <typename EnumType, EnumType... enumArgs>
struct EnumItemToStr
{
    constexpr static decltype(EnumItemArray<EnumType, enumArgs...>::array) array = EnumItemArray<EnumType, enumArgs...>::array;
    constexpr static size_t length = EnumItemArray<EnumType, enumArgs...>::length;

    static const char* Str(EnumType enumValue, int nStartIndex = 0, int nEndIndex = length)
    {
        return (nStartIndex >= nEndIndex || enumValue < array[nStartIndex].value ||  enumValue > array[nEndIndex - 1].value) ? "can't find enum value" :
                (array[nStartIndex].value == enumValue ? array[nStartIndex].str :
                    (array[nEndIndex - 1].value == enumValue ? array[nEndIndex - 1].str :
                        (enumValue < array[(nStartIndex + nEndIndex) / 2].value ? 
                            Str(enumValue, nStartIndex, (nStartIndex + nEndIndex) / 2) :
                            Str(enumValue, (nStartIndex + nEndIndex) / 2, nEndIndex))));
    }
};
template <typename EnumType, EnumType... enumArgs>
constexpr decltype(EnumItemArray<EnumType, enumArgs...>::array) EnumItemToStr<EnumType, enumArgs...>::array;

}