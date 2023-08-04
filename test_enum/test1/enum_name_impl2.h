#pragma once
#include <type_traits>
#include <map>
#include "arrow/typelist/type_name/type_name_impl.h"
#include "arrow/typelist/static_string.h"

namespace Arrow2
{
using namespace Arrow;
namespace details
{

#ifdef __clang__
constexpr char NoSocpedEnumNameStart[] = "t = ";
constexpr char NoSocpedEnumNameEnd[] = ", length =";
constexpr char NoSocpedEnumInVain[] = " = (";
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

// 核心代码用于获取枚举类型 [zhuyb 2023-08-04 11:29:37]
template<typename T, T t, int length = -1>
struct EnumItemName
{
    static void Trace()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    constexpr static StaticStr::StringView<length> Impl()
    {
        return details::IsSocpedEnum<T>::value ? SocpedEnumName() : NoSocpedEnumName();
    }

    constexpr static StaticStr::StringView<length> NoSocpedEnumName()
    {
        return StaticStr::StringView<length>(__PRETTY_FUNCTION__ + StaticStr::Find<1>(__PRETTY_FUNCTION__, details::NoSocpedEnumNameStart) + StaticStr::StrLen(details::NoSocpedEnumNameStart),
                                             typename MakeIntegerSequence<length>::type{});
    }

    constexpr static StaticStr::StringView<length> SocpedEnumName()
    {
        return StaticStr::StringView<length>(__PRETTY_FUNCTION__ + StaticStr::Find<0>(__PRETTY_FUNCTION__, details::SocpedEnumNameStart) + StaticStr::StrLen(details::SocpedEnumNameStart),
                                             typename MakeIntegerSequence<length>::type{});
    }
};

// 错误的枚举值 [zhuyb 2023-08-04 11:29:55]
template<typename T, T t>
struct EnumItemName<T, t, 0>
{
    static void Trace()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    constexpr static StaticStr::StringView<0> Impl()
    {
        return StaticStr::Str("");
    }
};

// 最初的入口 用于计算枚举字符串的长度 [zhuyb 2023-08-04 11:30:17]
template<typename T, T t>
struct EnumItemName<T, t, -1>
{
    static void Trace()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        EnumItemName<T, t, Length()>::Trace();
    }

    /**
     * @description: 判断值是否合法
     * @return {*}
     */
    constexpr static bool IsValid()
    {
        return StaticStr::Find<0>(__PRETTY_FUNCTION__, details::NoSocpedEnumInVain) == std::string::npos;
    }

    /**
     * @description: 获取枚举类型对应字符串的长度
     * @return {*}
     */
    constexpr static int Length()
    {
        return IsValid() ? (details::IsSocpedEnum<T>::value ? LengthScopedEnum() : LengthNoScopedEnum()) : 0;
    }
    
    constexpr static int LengthScopedEnum()
    {
        return StaticStr::Find<0>(__PRETTY_FUNCTION__, details::SocpedEnumNameEnd) - StaticStr::Find<0>(__PRETTY_FUNCTION__, details::SocpedEnumNameStart) - StaticStr::StrLen(details::SocpedEnumNameStart);
    }

    constexpr static int LengthNoScopedEnum()
    {
        return StaticStr::Find<0>(__PRETTY_FUNCTION__, details::NoSocpedEnumNameEnd) - StaticStr::Find<1>(__PRETTY_FUNCTION__, details::NoSocpedEnumNameStart) - StaticStr::StrLen(details::NoSocpedEnumNameStart);
    }

    /**
     * @description: 获取枚举值对应字符串
     * @return {*}
     */    
    constexpr static auto Impl() -> decltype(EnumItemName<T, t, EnumItemName<T, t>::Length()>::Impl())
    {
        return EnumItemName<T, t, EnumItemName<T, t>::Length()>::Impl();
    }

    // constexpr static auto Impl1() -> decltype(TypeName<T>::Impl() + StaticStr::Str("::") + EnumItemName<T, t>::Impl())
    // {
    //     return TypeName<T>::Impl() + StaticStr::Str("::") + EnumItemName<T, t>::Impl();
    // }

    constexpr static const char* Name()
    {
        return value.data;
    }

    // constexpr static const char* FullName()
    // {
    //     return long_value.data;
    // }

    static constexpr decltype(EnumItemName<T, t>::Impl()) value = EnumItemName<T, t>::Impl();
    // static constexpr decltype(EnumItemName<T, t>::Impl1()) long_value = EnumItemName<T, t>::Impl1();

};
template<typename T, T t>
constexpr decltype(EnumItemName<T, t>::Impl()) EnumItemName<T, t>::value;
// template<typename T, T t>
// constexpr decltype(EnumItemName<T, t>::Impl1()) EnumItemName<T, t>::long_value;

/////////////////////////////////////////////////////////////


template<std::size_t N>
struct ValidData
{
    std::size_t count=0;
    bool valid[N] = {false};

    template<typename E,  std::underlying_type<E> val, std::size_t Size, int Min>
    constexpr ValidData()

    template<typename E,  std::underlying_type<E> val
    constexpr void SetValid
};


namespace details {


template<typename EnumType>
struct EnumListData
{
    using type=std::map<EnumType, const char*>;
    static type value;
};
template<typename EnumType>
typename EnumListData<EnumType>::type EnumListData<EnumType>::value;

template <typename EnumType, EnumType _emFirst, EnumType _emLast, EnumType... enumTypes>
struct EnumListToStrImpl
{
    using value = EnumListData<EnumType>;

    static bool Init()
    {
        EnumListToStrImpl<EnumType, _emFirst, _emLast>::Init();
        EnumListToStrImpl<EnumType, enumTypes...>::Init();
        return true;
    }
};

template <typename EnumType, EnumType _emFirst, EnumType _emLast>
struct EnumListToStrImpl<EnumType, _emFirst, _emLast>
{
    using value = EnumListData<EnumType>;

    static bool Init()
    {
        EnumListToStrImpl<EnumType, _emFirst, _emFirst>::Init();
        EnumListToStrImpl<EnumType, static_cast<EnumType>(_emFirst + 1), _emLast>::Init();
        return true;
    }
};

template <typename EnumType, EnumType _emEnd>
struct EnumListToStrImpl<EnumType, _emEnd, _emEnd>
{
    using item = EnumItemName<EnumType, _emEnd>;
    using value = EnumListData<EnumType>;

    static bool Init()
    {
        value::value[_emEnd] = item::Name();
        return true;
    }
};
}

template <typename EnumType, EnumType... enumArgs>
struct EnumListToStr
{
    static_assert(sizeof...(enumArgs) % 2 == 0 , "枚举参数的个数必须为偶数个，首尾成对出现");
    using type = details::EnumListToStrImpl<EnumType, enumArgs...>;
    using value = details::EnumListData<EnumType>;

    // static bool gInit;
    static const char* ItemStr(EnumType enumValue)
    {
        static bool bInit = type::Init();
        auto it = value::value.find(enumValue);
        if(it == value::value.end())
        {
            return "";
        }
        return it->second;
    }
};
// template <typename EnumType, EnumType... enumArgs>
// bool EnumListToStr<EnumType, enumArgs...>::gInit = EnumListToStr<EnumType, enumArgs...>::type::Init();




}