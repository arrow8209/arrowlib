#pragma once
#include "..//make_integer_sequence.h"
#include <map>
#include <stdint.h>
#include <string>

namespace Arrow
{

namespace StaticStr
{

template <size_t index>
constexpr char at(const char* a) { return a[index]; }

template<std::size_t M, std::size_t N>
constexpr const char get(const char (&arr)[N])
{
    return arr[ M < N ? M : N-1];
}

template <size_t size>
struct StringView
{
    const char data[size + 1];
    const size_t length = 0;
    constexpr StringView() : data{}, length(0) {}

    constexpr StringView(const char* str) : StringView(str, typename Arrow::MakeIntegerSequence<size>::type{}) {}

    template <int... args>
    constexpr StringView(const char* str, Arrow::IntegerSequence<args...>) : data{at<args>(str)..., 0}, length(sizeof...(args)) {}

    template <int... args1, int... args2>
    constexpr StringView(const char* sz1, Arrow::IntegerSequence<args1...>, const char* sz2, Arrow::IntegerSequence<args2...>)
        : data{at<args1>(sz1)..., at<args2>(sz2)..., 0}, length(sizeof...(args1) + sizeof...(args2)) {}

    template <size_t size1>
    constexpr StringView<size + size1> operator+(StringView<size1> b) const
    {
        return StringView<size + size1>(data, typename Arrow::MakeIntegerSequence<size>::type{},
                                        b.data, typename Arrow::MakeIntegerSequence<size1>::type{});
    }


    // template<int ...args>
    // constexpr StringView(Arrow::IntegerSequence<args...>, charView<args>... argsChar) : data{argsChar..., 0} {}
    // template <int size1, int size2>
    // constexpr StringView(StringView<size1> sv1, StringView<size2> sv2)
    //    : StringView(sv1, typename Arrow::MakeIntegerSequence<size2>::type{}, sv2.data) {}
    // template <int size1, int ...args>
    // constexpr StringView(StringView<size1> sv1, Arrow::IntegerSequence<args...>,const char* sz2)
    //     : StringView(typename Arrow::MakeIntegerSequence<size1>::type{}, sv1.data, at<args>(sz2)...) {}
    // template <int ...args, typename ...CHS>
    // constexpr StringView(Arrow::IntegerSequence<args...>, const char* sz1, CHS ...chs )
    //     : data{at<args>(sz1)..., chs..., 0} {}
    // template <int size1, int size2>
    // constexpr StringView(StringView<size1> sv1, StringView<size2> sv2)
    //    : StringView(typename Arrow::MakeIntegerSequence<size1>::type{}, sv1.data, typename Arrow::MakeIntegerSequence<size2>::type{}, sv2.data) {}

};

namespace details
{

struct StrLenImpl
{
    static constexpr size_t Impl(const char* sz, size_t nLength = 0)
    {
       return (*sz != 0) ? StrLenImpl::Impl(sz + 1, nLength + 1) : nLength;
    }
};
}

constexpr size_t StrLen(const char* sz)
{
    return details::StrLenImpl::Impl(sz);
}

template<size_t length>
constexpr StringView<length - 1> Str(const char (&a)[length])
{
    return StringView<length - 1>(a);
}


}
}
