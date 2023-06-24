#pragma once
#include <stdint.h>
#include <map>
#include <string>
#include "string_view.h"

namespace Arrow
{

namespace StaticStr
{

namespace details
{

template <size_t start, size_t length = size_t(-1)>
struct SubStrImpl
{
    template<size_t N>
    static constexpr StringView<length> Impl(const char (&sz)[N])
    {
       return StringView<length>(sz + start, typename Arrow::MakeIntegerSequence<length>::type{});
    }
};

template <size_t start>
struct SubStrImpl<start, size_t(-1)>
{
    template<size_t N>
    static constexpr StringView<N - start - 1> Impl(const char (&sz)[N])
    {
       return StringView<N - start - 1>(sz + start, typename Arrow::MakeIntegerSequence<N - start - 1>::type{});
    }
};
}

template<size_t start, size_t length = size_t(-1), size_t N>
constexpr auto SubStr(const char (&sz)[N]) -> decltype(details::SubStrImpl<start, length>::Impl(sz))
{
    return details::SubStrImpl<start, length>::Impl(sz);
}

template<size_t start, size_t length = size_t(-1), size_t N>
constexpr auto SubStr(const StringView<N> sz) ->decltype(SubStr<start, length>(sz.data))
{
    return SubStr<start, length>(sz.data);
}

}
}

