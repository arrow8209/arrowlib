#pragma once
#include "typelist_fun.h"
#include "make_integer_sequence.h"

namespace Arrow
{

namespace StaticStr
{

namespace details
{

template <int index>
constexpr char at(const char* a) { return a[index]; }

template<typename T>
struct SubStrImpl;

template<int ...args>
struct SubStrImpl<Arrow::IntegerSequence<args...>>
{
    const char data[sizeof...(args) + 1];
    constexpr SubStrImpl() : data{0} {}
    constexpr SubStrImpl(const char* a) : data{at<args>(a)..., 0} {}
    constexpr SubStrImpl(const char* a ,int nStart) : data{at<args>(a + nStart)..., 0} {}
};

constexpr int FindImpl(const char* sz, char ch, int index)
{
    return (*sz) ? ((*sz == ch) ? index : FindImpl(sz + 1, ch, index + 1)) : -1;
}

constexpr int FindLastImpl(const char* sz, char ch, int index, int nLastIndex)
{
    return (*sz) ? ((*sz == ch) ? FindLastImpl(sz + 1, ch, index + 1, index) : FindLastImpl(sz + 1, ch, index + 1, nLastIndex)) : nLastIndex;
}

}

constexpr int Find(const char* sz, char ch)
{
    return details::FindImpl(sz, ch, 0);
}

constexpr int FindLast(const char* sz, char ch)
{
    return details::FindLastImpl(sz, ch, 0, -1);
}

template<int start, int length>
constexpr auto SubStr(const char* sz) -> decltype(details::SubStrImpl<typename Arrow::MakeIntegerSequence<length>::type>{})
{
    return details::SubStrImpl<typename Arrow::MakeIntegerSequence<length>::type>(sz, start);
}



// namespace static_string
// {
// typedef long long hash64;
// namespace const_expr
// {
// constexpr hash64 prime = 0x100000001B3ull;
// constexpr hash64 basis = 0xCBF29CE484222325ull;
// }
// constexpr hash64 make_hash_static(char const* str)
// {
//     return (*str) ? (*(str + 1)) ? (((*str) * const_expr::prime + const_expr::basis) ^ make_hash_static(str + 1)) : ((*str) * const_expr::prime + const_expr::basis) : 0;
// }

// constexpr hash64 operator"" _hash(char const* p, size_t)
// {
//     return make_hash_static(p);
// }
// template <char>
// using charDummy = char;

// template <int N>
// constexpr char at(const char* a) { return a[N]; }

// template <int... dummy>
// struct F
// {
//     const char Name[sizeof...(dummy) + 1];
//     const hash64 Hash;
//     const int Length;
//     const int Size;

//     constexpr F(const char* a) : Name{at<dummy>(a)..., 0}, Length(sizeof...(dummy)), Size(sizeof...(dummy) + 1), Hash(a[0] * const_expr::prime + const_expr::basis) {}
//     constexpr F(hash64 h, charDummy<dummy>... a) : Name{a..., 0}, Length(sizeof...(dummy)), Size(sizeof...(dummy) + 1), Hash(h) {}
//     constexpr F(const F& a) : Name{a.Name[dummy]..., 0}, Length(a.Length), Size(a.Size), Hash(a.Hash) {}

//     template <int... dummyB>
//     constexpr F<dummy..., sizeof...(dummy) + dummyB...> operator+(F<dummyB...> b) const
//     {
//         return {this->Hash ^ b.Hash, this->Name[dummy]..., b.Name[dummyB]...};
//     }
//     operator const char*() const { return Name; }
// };

// template <int I>
// struct get_string
// {
//     constexpr static auto g(const char* a) -> decltype(get_string<I - 1>::g(a) + F<0>(a + I))
//     {
//         return get_string<I - 1>::g(a) + F<0>(a + I);
//     }
// };

// template <>
// struct get_string<0>
// {
//     constexpr static F<0> g(const char* a)
//     {
//         return {a};
//     }
// };

// template <int I>
// constexpr auto str(const char (&a)[I]) -> decltype(get_string<I - 2>::g(a))
// {
//     return get_string<I - 2>::g(a);
// }

}
}