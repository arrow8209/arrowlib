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

// C++17的写法 [zhuyb 2023-06-23 21:31:15]
struct FindStrImplAssist
{
private:
    template<size_t N, int ...args>
    static constexpr bool Equal(const char* str1, const char (&str2)[N], Arrow::IntegerSequence<args...>)
    {
        return ((at<args>(str1) == at<args>(str2)) && ... && true);
    }

public:
    template<size_t N>
    static constexpr bool Equal(const char* str1, const char (&str2)[N])
    {
        return StrLenImpl::Impl(str1, 0) < N - 1 ? false : Equal(str1, str2, typename Arrow::MakeIntegerSequence<N - 1>::type{});
    }
};


// 查找第 times 次出现的位置 0:查找最后一次出现的位置  1:查找第一次出现的位置 N:查找第N次出现的位置(如果没有就返回最后一次出现的位置)[zhuyb 2023-06-21 22:31:48]
template<size_t times>
struct FindImpl
{
    static constexpr size_t Impl(const char* str, const char ch, size_t index = 0, size_t pos = -1)
    {
        return (str[index] == 0) ?  pos : ((str[index] == ch) ? 
                                            FindImpl<times - 1>::Impl(str, ch, index + 1, index) : 
                                            FindImpl<times>::Impl(str, ch, index + 1, pos));
    }

    template <size_t N>
    static constexpr size_t Impl(const char* str1, const char (&str2)[N], size_t index = 0, size_t pos = -1)
    {
        return (str1[index] == 0) ? pos : (FindStrImplAssist::Equal(str1 + index, str2) ? 
                                            FindImpl<times - 1>::Impl(str1, str2, index + 1, index) : 
                                            FindImpl<times>::Impl(str1, str2, index + 1, pos));
    }

};

template<>
struct FindImpl<1>
{
    static constexpr size_t Impl(const char* str, const char ch, size_t index = 0, size_t pos = -1)
    {
        return (str[index] == 0) ?  pos : ((str[index] == ch) ? 
                                            index : 
                                            FindImpl<1>::Impl(str, ch, index + 1, pos));
    }

    template<size_t N>
    static constexpr size_t Impl(const char* str1, const char (&str2)[N], size_t index = 0, size_t pos = -1)
    {
        return (str1[index] == 0) ? pos : (FindStrImplAssist::Equal(str1 + index, str2) ? 
                                            index : 
                                            FindImpl<1>::Impl(str1, str2, index + 1, pos));
    }

};

template<>
struct FindImpl<0>
{

    static constexpr size_t Impl(const char* str,const char ch, size_t index = 0, size_t pos = -1)
    {
        return (str[index] == 0) ? pos : ((str[index] == ch) ? 
                                            FindImpl<0>::Impl(str, ch, index + 1, index) : 
                                            FindImpl<0>::Impl(str, ch, index + 1, pos));
    }

    template<size_t N>
    static constexpr size_t Impl(const char* str1, const char (&str2)[N], size_t index = 0, size_t pos = -1)
    {
        return (str1[index] == 0) ? pos : (FindStrImplAssist::Equal(str1 + index, str2) ? 
                                        FindImpl<0>::Impl(str1, str2, index + 1, index) : 
                                        FindImpl<0>::Impl(str1, str2, index + 1, pos));
    }

};

}
// 查找第 times 次出现的位置 0:查找最后一次出现的位置  1:查找第一次出现的位置 N:查找第N次出现的位置(如果没有就返回最后一次出现的位置)[zhuyb 2023-06-21 22:31:48]
constexpr static size_t FindTypeFirst = 1;
constexpr static size_t FindTypeLast = 0;
template<size_t times = 1>
constexpr size_t Find(const char* sz, const char ch, int startPos = 0)
{
    return details::FindImpl<times>::Impl(sz, ch, startPos);
}

template<size_t times = 1, size_t N>
constexpr size_t Find(const char* str1, const char (&str2)[N], int startPos = 0)
{
    return details::FindImpl<times>::Impl(str1, str2, startPos);
}

template<size_t times = 1, size_t size>
constexpr size_t Find(const StringView<size> sv,const  char ch, int startPos = 0)
{
    return Find<times>(sv.data, ch, startPos);
}

template<size_t times = 1, size_t size, size_t N>
constexpr size_t Find(const StringView<size> sv, const char (&str)[N], int startPos = 0)
{
    return Find<times>(sv.data, str, startPos);
}

constexpr size_t FindLast(const char* sz,const char ch, int startPos = 0)
{
    return Find<0>(sz, ch, startPos);
}

template<size_t size>
constexpr size_t FindLast(const StringView<size> sv,const char ch, int startPos = 0)
{
    return Find<0>(sv.data, ch, startPos);
}

template<size_t N>
constexpr size_t FindLast(const char* sz, const char (&str)[N], int startPos = 0)
{
    return Find<0>(sz, str, startPos);
}

template<size_t size, size_t N>
constexpr size_t FindLast(const StringView<size> sv, const char (&str)[N], int startPos = 0)
{
    return Find<0>(sv.data, str, startPos);
}

}
}
