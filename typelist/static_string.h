#pragma once
#include "typelist_fun.h"

namespace Arrow
{

namespace static_string
{

template<std::size_t M, std::size_t N>
constexpr const char get(const char (&arr)[N])
{
    return arr[ M < N ? M : N-1];
}

// 最长支持1024个字符 [zhuyb 2022-07-30 20:25:54]
#define MakeCharSequence_16(m, n, str) Arrow::static_string::get<0x##m##n##0>(str), Arrow::static_string::get<0x##m##n##1>(str), \
                                       Arrow::static_string::get<0x##m##n##2>(str), Arrow::static_string::get<0x##m##n##3>(str), \
                                       Arrow::static_string::get<0x##m##n##4>(str), Arrow::static_string::get<0x##m##n##5>(str), \
                                       Arrow::static_string::get<0x##m##n##6>(str), Arrow::static_string::get<0x##m##n##7>(str), \
                                       Arrow::static_string::get<0x##m##n##8>(str), Arrow::static_string::get<0x##m##n##9>(str), \
                                       Arrow::static_string::get<0x##m##n##a>(str), Arrow::static_string::get<0x##m##n##b>(str), \
                                       Arrow::static_string::get<0x##m##n##c>(str), Arrow::static_string::get<0x##m##n##d>(str), \
                                       Arrow::static_string::get<0x##m##n##e>(str), Arrow::static_string::get<0x##m##n##f>(str)

#define MakeCharSequence_64(m, str) MakeCharSequence_16(m, 0, str), MakeCharSequence_16(m, 1, str), \
                                    MakeCharSequence_16(m, 2, str), MakeCharSequence_16(m, 3, str), \
                                    MakeCharSequence_16(m, 4, str), MakeCharSequence_16(m, 5, str), \
                                    MakeCharSequence_16(m, 6, str), MakeCharSequence_16(m, 7, str), \
                                    MakeCharSequence_16(m, 8, str), MakeCharSequence_16(m, 9, str), \
                                    MakeCharSequence_16(m, a, str), MakeCharSequence_16(m, b, str), \
                                    MakeCharSequence_16(m, c, str), MakeCharSequence_16(m, d, str), \
                                    MakeCharSequence_16(m, e, str), MakeCharSequence_16(m, f, str)

#define MakeCharSequence_128(str) MakeCharSequence_64(0, str), \
                                  MakeCharSequence_64(1, str)

#define MakeCharSequence_256(str) MakeCharSequence_64(0, str), \
                                  MakeCharSequence_64(1, str), \
                                  MakeCharSequence_64(2, str)

#define MakeCharSequence_512(str) MakeCharSequence_64(0, str), \
                                  MakeCharSequence_64(1, str), \
                                  MakeCharSequence_64(2, str), \
                                  MakeCharSequence_64(3, str)

#define MakeCharSequence_1024(str) MakeCharSequence_64(0, str), \
                                   MakeCharSequence_64(1, str), \
                                   MakeCharSequence_64(2, str), \
                                   MakeCharSequence_64(3, str), \
                                   MakeCharSequence_64(4, str)

#define STATIC_STRING(str) Arrow::typelist::splite<sizeof(str) - 1, Arrow::typelist::tvalue_typelist<char, MakeCharSequence_1024(str)>>::Head

#define STATIC_FILE  Arrow::static_string::getfilename<STATIC_STRING(__FILE__)>::type

namespace details
{
template<int index, typename StaticStr>
struct getfilename;

template<typename StaticStr>
struct getfilename<-1, StaticStr>
{
    typedef StaticStr type;
};

template<int index, typename ...Args>
struct getfilename<index, typelist::typelist<Args...> >
{
protected:
    static_assert(index >= 0, "index 小于0（请检查代码逻辑）");
    typedef typename typelist::splite<index + 1, typelist::typelist<Args...>> split_str;

public:
    typedef typename split_str::Tail type;
};

}

template<typename StaticStr>
struct getfilename;

template<typename ... Args>
struct getfilename<typelist::typelist<Args...>>
{
protected:
    typedef typename typelist::typelist<Args...> static_str;
    typedef typename typelist::find_last<typelist::tvalue_type<char, '/'>, static_str> find_last_forward_slash;
    
public:
    typedef typename details::getfilename<find_last_forward_slash::value, static_str>::type type;
};

}


}