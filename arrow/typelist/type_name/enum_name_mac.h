#pragma once
#include <type_traits>
#include "type_name_mac.h"
#include "../static_string2.h"

namespace Arrow
{

template<typename T, T t, int length = -1>
struct EnumItemName
{
    static void Trace()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    constexpr static StaticStr::StringView<length> Impl()
    {
        return StringView<length>(typename MakeIntegerSequence<length>::type{},
                                  __PRETTY_FUNCTION__ + StaticStr::Find<1>(__PRETTY_FUNCTION__, "t = ") + StaticStr::StrLen("t = "));
    }
};

template<typename T, T t>
struct EnumItemName<T, t, -1>
{
    static void Trace()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;

        constexpr int start1 = StaticStr::Find<1>(__PRETTY_FUNCTION__, "T = ") + StaticStr::StrLen("T = ");
        constexpr int end1 = StaticStr::Find<1>(__PRETTY_FUNCTION__, ", t = ");
        constexpr int length1 = end1 - start1;
        constexpr auto svName1 = StaticStr::StringView<length1>(typename MakeIntegerSequence<length1>::type{},
                                  __PRETTY_FUNCTION__ + start1);

        constexpr int start2 = StaticStr::Find<1>(__PRETTY_FUNCTION__, ", t = ") + StaticStr::StrLen(", t = ");
        constexpr int end2 = StaticStr::Find<1>(__PRETTY_FUNCTION__, ", length = ");
        constexpr int length2 = end2 - start2;
        constexpr auto svName2 = StaticStr::StringView<length1>(typename MakeIntegerSequence<length2>::type{},
                                  __PRETTY_FUNCTION__ + start2);


        std::cout << svName1.data << std::endl;
        std::cout << svName2.data << std::endl;
        EnumItemName<T, t, 0>::Trace();
    }

    constexpr static int Length()
    {
        return StaticStr::Find<1>(__PRETTY_FUNCTION__, ", length = ") - StaticStr::Find<1>(__PRETTY_FUNCTION__, "t = ")  - StaticStr::StrLen("t = ");
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