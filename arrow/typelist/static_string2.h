#pragma once
#include <type_traits>
#include "make_integer_sequence.h"

namespace Arrow
{

namespace StaticStr
{

template <int index>
constexpr char at(const char* a) { return a[index]; }

template <int size>
struct StringView
{
    const char data[size + 1];
    const int length = size;
    constexpr StringView() : data{} {}

    constexpr StringView(const char* str) : StringView(typename MakeIntegerSequence<size>::type{}, str) {}

    template <int... args>
    constexpr StringView(IntegerSequence<args...>, const char* str) : data{at<args>(str)..., 0} {}

    // template<int ...args>
    // constexpr StringView(IntegerSequence<args...>, charView<args>... argsChar) : data{argsChar..., 0} {}
    // template <int size1, int size2>
    // constexpr StringView(StringView<size1> sv1, StringView<size2> sv2) 
    //    : StringView(sv1, typename MakeIntegerSequence<size2>::type{}, sv2.data) {}
    // template <int size1, int ...args>
    // constexpr StringView(StringView<size1> sv1, IntegerSequence<args...>,const char* sz2)
    //     : StringView(typename MakeIntegerSequence<size1>::type{}, sv1.data, at<args>(sz2)...) {}
    // template <int ...args, typename ...CHS>
    // constexpr StringView(IntegerSequence<args...>, const char* sz1, CHS ...chs )
    //     : data{at<args>(sz1)..., chs..., 0} {}
    // template <int size1, int size2>
    // constexpr StringView(StringView<size1> sv1, StringView<size2> sv2) 
    //    : StringView(typename MakeIntegerSequence<size1>::type{}, sv1.data, typename MakeIntegerSequence<size2>::type{}, sv2.data) {}

    template <int... args1, int... args2>
    constexpr StringView(IntegerSequence<args1...>, const char* sz1, IntegerSequence<args2...>, const char* sz2)
        : data{at<args1>(sz1)..., at<args2>(sz2)..., 0} {}

    template <int size1>
    constexpr StringView<size + size1> operator+(StringView<size1> b) const
    {
       return StringView<size + size1>(typename MakeIntegerSequence<size>::type{}, data,
                                       typename MakeIntegerSequence<size1>::type{}, b.data);
    }

};

namespace details
{

constexpr int FindImpl(const char* sz, char ch, int index)
{
    return (*sz != 0) ? ((*sz == ch) ? index : FindImpl(sz + 1, ch, index + 1)) : -1;
}

constexpr int FindLastImpl(const char* sz, char ch, int index, int nLastIndex)
{
    return (*sz != 0) ? ((*sz == ch) ? FindLastImpl(sz + 1, ch, index + 1, index) : FindLastImpl(sz + 1, ch, index + 1, nLastIndex)) : nLastIndex;
}

constexpr int StrLenImpl(const char* sz, int nLength)
{
    return (*sz != 0) ? StrLenImpl(sz + 1, nLength + 1) : nLength;
}

template <int start, int length>
struct SubStrImpl
{
    static constexpr StringView<length> Impl(const char* sz)
    {
       return StringView<length>(typename Arrow::MakeIntegerSequence<length>::type{}, sz + start);
    }
};

}

constexpr int Find(const char* sz, char ch)
{
    return details::FindImpl(sz, ch, 0);
}

template<int length>
constexpr int Find(const StringView<length> sv, char ch)
{
    return details::FindImpl(sv.data, ch, 0);
}

constexpr int FindLast(const char* sz, char ch)
{
    return details::FindLastImpl(sz, ch, 0, -1);
}

template<int length>
constexpr int FindLast(const StringView<length> sv, char ch)
{
    return details::FindLastImpl(sv.data, ch, 0, -1);
}

constexpr int StrLen(const char* sz)
{
    return details::StrLenImpl(sz, 0);
}

template<int start, int length, int szSize>
constexpr auto SubStr(const char (&sz)[szSize]) -> decltype(details::SubStrImpl<start, length>::Impl(sz))
{
    return details::SubStrImpl<start, length>::Impl(sz);
}

template<int start, int szSize>
constexpr auto SubStr(const char (&sz)[szSize]) -> decltype(details::SubStrImpl<start, szSize - start>::Impl(sz))
{
    return details::SubStrImpl<start, szSize - start>::Impl(sz);
}

template<int start, int length, int szLength>
constexpr auto SubStr(const StringView<szLength> sz) ->decltype(SubStr<start, length>(sz.data))
{
    return SubStr<start, length>(sz.data);
}

template<int start, int svSize>
constexpr auto SubStr(const StringView<svSize> sv) ->decltype(SubStr<start, svSize-start>(sv.data))
{
    return SubStr<start, svSize-start>(sv.data);
}

template<int length>
constexpr StringView<length - 1> Str(const char (&a)[length])
{
    return StringView<length - 1>(a);
}


}
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