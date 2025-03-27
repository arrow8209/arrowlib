#pragma once
#include <type_traits>
#include <map>
#include "type_name_impl.h"
#include "../static_string.h"

namespace Arrow
{

namespace details
{

#ifdef __clang__
constexpr char enumNameStart[] = "t = ";
constexpr char enumNameEnd[] = "]";
#elif __GNUC__
constexpr char enumNameStart[] = "t = ";
constexpr char enumNameEnd[] = "]";
#else
#endif

// 判断枚举是 class enum 还是 enum
template <typename EnumType, typename = typename std::enable_if<std::is_enum<EnumType >::value>::type>
struct IsScopedEnum : std::false_type
{
};

template <typename EnumType>
struct IsScopedEnum<EnumType , typename std::enable_if<!std::is_convertible<EnumType , int>::value>::type> : std::true_type
{
};

}

/////////////////////////////////////////////////////////////
template<typename T, T t>
struct EnumItemName
{
private:
    /**
     * @description: 获取枚举英文名长度仅枚举部分
     * @return {*}
     */
    constexpr static int Length()
    {
        return StaticStr::Find<0>(__PRETTY_FUNCTION__, details::enumNameEnd) -
               StaticStr::Find<1>(__PRETTY_FUNCTION__, details::enumNameStart) -
               StaticStr::StrLen(details::enumNameStart) - 
               PrefixLength();
    }

    /**
     * @description: 获取枚举前缀长度
     * @return {*}
     */
    constexpr static int PrefixLength()
    {
        return StaticStr::Find<0>(__PRETTY_FUNCTION__, "::") > StaticStr::Find<1>(__PRETTY_FUNCTION__, details::enumNameStart)
                   ? StaticStr::Find<0>(__PRETTY_FUNCTION__, "::") -
                         StaticStr::Find<1>(__PRETTY_FUNCTION__, details::enumNameStart) -
                         StaticStr::StrLen(details::enumNameStart) + 2
                   : 0;
    }

    /**
     * @description: 获取枚举短名
     * @return {*}
     */
    constexpr static StaticStr::StringView<Length()> ImplShorName()
    {
        return StaticStr::StringView<Length()>(__PRETTY_FUNCTION__ +
                                             StaticStr::Find<1>(__PRETTY_FUNCTION__, details::enumNameStart) +
                                             StaticStr::StrLen(details::enumNameStart) + PrefixLength());
    }

    /**
     * @description: 获取枚举完整名称
     * @return {*}
     */    
    constexpr static auto ImplLongName() -> decltype(TypeName<T>::Impl() + StaticStr::Str("::") + EnumItemName<T, t>::ImplShorName())
    {
        return TypeName<T>::Impl() + StaticStr::Str("::") + EnumItemName<T, t>::ImplShorName();
    }

public:
    // 调试 [zhuyb 2023-10-17 16:17:32]
    static int Trace3()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return 0;
    }

    static StaticStr::StringView<Length()> Trace2()
    {
        return StaticStr::StringView<Length()>();
    }
    static int Trace1()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::cout << Length() << ":" << PrefixLength() << std::endl;
        Trace2();
        return 0;
    }

    constexpr static const char* ShortName()
    {
        return shortName.data;
    }

    constexpr static const char* LongName()
    {
        return longNalue.data;
    }

private:
    static inline constexpr decltype(EnumItemName<T, t>::ImplShorName()) shortName = EnumItemName<T, t>::ImplShorName();
    static inline constexpr decltype(EnumItemName<T, t>::ImplLongName()) longNalue = EnumItemName<T, t>::ImplLongName();
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

template <typename EnumType, 
            typename std::underlying_type<EnumType>::type _emFirst,
            typename std::underlying_type<EnumType>::type _emLast,
            typename std::underlying_type<EnumType>::type... enumTypes>
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

template <typename EnumType,
          typename std::underlying_type<EnumType>::type _emFirst,
          typename std::underlying_type<EnumType>::type _emLast>
struct EnumListToStrImpl<EnumType, _emFirst, _emLast>
{
    using value = EnumListData<EnumType>;

    static bool Init()
    {
        EnumListToStrImpl<EnumType, _emFirst, _emFirst>::Init();
        EnumListToStrImpl<EnumType, _emFirst + 1, _emLast>::Init();
        return true;
    }
};

template <typename EnumType, typename std::underlying_type<EnumType>::type _emEnd>
struct EnumListToStrImpl<EnumType, _emEnd, _emEnd>
{
    using item = EnumItemName<EnumType, static_cast<EnumType>(_emEnd)>;
    using value = EnumListData<EnumType>;

    static bool Init()
    {
        value::value[static_cast<EnumType>(_emEnd)] = item::ShortName();
        return true;
    }
};
}

template <typename EnumType, EnumType... enumArgs>
struct EnumListToStr
{
    static_assert(sizeof...(enumArgs) % 2 == 0 , "枚举参数的个数必须为偶数个，首尾成对出现");
    using enumValueType = typename std::underlying_type<EnumType>::type;
    using type = details::EnumListToStrImpl<EnumType, static_cast<enumValueType>(enumArgs)...>;
    using value = details::EnumListData<EnumType>;


    static const char* ItemStr(EnumType enumValue)
    {
        static bool bInit = type::Init();
        auto it = value::value.find(enumValue);
        if(it == value::value.end())
        {
            return "(unknow enum)";
        }
        return it->second;
    }
};
}