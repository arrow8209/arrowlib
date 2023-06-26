#pragma once
#include <type_traits>
#include "../static_string2.h"

namespace Arrow
{

namespace details
{

#ifdef __clang__
constexpr char typeNameStart[] = "T = ";
constexpr char typeNameEnd[] = ", length =";
#elif __GNUC__
constexpr char typeNameStart[] = "T = ";
constexpr char typeNameEnd[] = "]";
#else
#endif

}


template<typename T, int length = -1>
struct TypeName
{
    static void Trace()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    constexpr static StaticStr::StringView<length> Impl()
    {
        return StaticStr::StringView<length>(__PRETTY_FUNCTION__ + StaticStr::Find<1>(__PRETTY_FUNCTION__, details::typeNameStart) + StaticStr::StrLen(details::typeNameStart),
                                 typename MakeIntegerSequence<length>::type{});
    }
};

template<typename T>
struct TypeName<T, -1>
{

private:
    constexpr static int Length()
    {
        return StaticStr::Find<0>(__PRETTY_FUNCTION__,details::typeNameEnd) - StaticStr::Find<1>(__PRETTY_FUNCTION__, details::typeNameStart) - StaticStr::StrLen(details::typeNameStart);
    }

public:
    static void Trace()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        TypeName<T, 0>::Trace();
    }

    constexpr static auto Impl() -> decltype(TypeName<T, TypeName<T>::Length()>::Impl())
    {
        return TypeName<T, TypeName<T>::Length()>::Impl();
    }

    constexpr static const char* Name()
    {
        return value.data;
    }

    static constexpr decltype(TypeName<T>::Impl()) value = TypeName<T>::Impl();
};
template <typename T>
constexpr decltype(TypeName<T>::Impl()) TypeName<T>::value;


template<typename T>
constexpr const char* GetTypeName(T t)
{
    return TypeName<typename std::remove_cv<T>::type>::Name();
}

// 如果不想使用编辑器的名称可以使用自定义的名字,可以参照下面的写法 [zhuyb 2023-06-23 20:30:10]
// template<>
// struct TypeName<int, -1>
// {
//     constexpr static const char* Name()
//     {
//         return value.data;
//     }
//     static constexpr decltype(StaticStr::Str("int")) value = StaticStr::Str("int");
// };
// template<>
// constexpr decltype(StaticStr::Str("int")) TypeName<int>::value;




}