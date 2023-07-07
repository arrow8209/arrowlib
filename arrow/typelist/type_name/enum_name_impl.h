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
constexpr char enumNameEnd[] = ", length =";
#elif __GNUC__
constexpr char enumNameStart[] = "t = ";
constexpr char enumNameEnd[] = "]";
#else
#endif

}

template<typename T, T t, int length = -1>
struct EnumItemName
{
    static void Trace()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    constexpr static StaticStr::StringView<length> Impl()
    {
        return StaticStr::StringView<length>(__PRETTY_FUNCTION__ + StaticStr::Find<1>(__PRETTY_FUNCTION__, details::enumNameStart) + StaticStr::StrLen(details::enumNameStart),
                                             typename MakeIntegerSequence<length>::type{});
    }
};

template<typename T, T t>
struct EnumItemName<T, t, -1>
{
    static void Trace()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        EnumItemName<T, t, 0>::Trace();
    }

    constexpr static int Length()
    {
        return StaticStr::Find<0>(__PRETTY_FUNCTION__, details::enumNameEnd) - StaticStr::Find<1>(__PRETTY_FUNCTION__, details::enumNameStart)  - StaticStr::StrLen(details::enumNameStart);
    }
    
    /**
     * @description: 获取枚举值对应字符串
     * @return {*}
     */    
    constexpr static auto Impl() -> decltype(EnumItemName<T, t, EnumItemName<T, t>::Length()>::Impl())
    {
        return EnumItemName<T, t, EnumItemName<T, t>::Length()>::Impl();
    }

    constexpr static auto Impl1() -> decltype(TypeName<T>::Impl() + StaticStr::Str("::") + EnumItemName<T, t>::Impl())
    {
        return TypeName<T>::Impl() + StaticStr::Str("::") + EnumItemName<T, t>::Impl();
    }

    constexpr static const char* Name()
    {
        return value.data;
    }

    constexpr static const char* FullName()
    {
        return long_value.data;
    }

    static constexpr decltype(EnumItemName<T, t>::Impl()) value = EnumItemName<T, t>::Impl();
    static constexpr decltype(EnumItemName<T, t>::Impl1()) long_value = EnumItemName<T, t>::Impl1();

};
template<typename T, T t>
constexpr decltype(EnumItemName<T, t>::Impl()) EnumItemName<T, t>::value;
template<typename T, T t>
constexpr decltype(EnumItemName<T, t>::Impl1()) EnumItemName<T, t>::long_value;

/////////////////////////////////////////////////////////////


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