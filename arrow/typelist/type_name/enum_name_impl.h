#pragma once
#include <type_traits>
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
        return value1.data;
    }

    static constexpr decltype(EnumItemName<T, t>::Impl()) value = EnumItemName<T, t>::Impl();
    static constexpr decltype(EnumItemName<T, t>::Impl1()) value1 = EnumItemName<T, t>::Impl1();

};
template<typename T, T t>
constexpr decltype(EnumItemName<T, t>::Impl()) EnumItemName<T, t>::value;
template<typename T, T t>
constexpr decltype(EnumItemName<T, t>::Impl1()) EnumItemName<T, t>::value1;


}