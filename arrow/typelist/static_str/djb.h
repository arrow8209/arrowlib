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

struct StrToDJBImpl
{
    static constexpr uint32_t prime = 49999;
    static constexpr uint32_t Impl(const char* ch)
    {
        return ch[0] == 0 ? 0 : Impl(ch, 49999);
    }

    static constexpr uint32_t Impl(const char* ch, uint32_t value)
    {
        return ch[0] == 0 ? value : Impl(ch + 1, (value << 5) + value + ch[0]);
    }
};

}

static constexpr uint32_t StrToUInt32(const char* ch)
{
    return details::StrToDJBImpl::Impl(ch);
}

template<size_t size>
constexpr size_t StrToUInt32(const StringView<size>& sv)
{
    return details::StrToDJBImpl::Impl(sv.data);
}

// template<typename ...Args>
// static uint32_t StrToUInt32(const char* sz, Args... args)
// {
//      return details::StrToDJBImpl::Impl(sz, StrToUInt32(args...));
// }

// template<size_t size, typename ...Args>
// static uint32_t StrToUInt32(const StringView<size>& sv, Args... args)
// {
//      return details::StrToDJBImpl::Impl(sv.data, StrToUInt32(args...));
// }
}
}