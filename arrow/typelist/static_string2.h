#pragma once
#include "typelist_fun.h"
#include "make_integer_sequence.h"

namespace Arrow
{


template <int index>
constexpr char at(const char* a) { return a[index]; }

template <int index, int length>
constexpr char at1(const char(&a)[length]) { return a[index]; }

template<int>
using charView=char;

template <int ...args>
struct StringView
{
    const char data[sizeof...(args) + 1];
    // const int length = size;
    constexpr StringView() : data{} {}

    constexpr StringView(const char* str) : data{at<args>(str)..., 0} {}
    // constexpr StringView(const char* str) : StringView(str, typename Arrow::MakeIntegerSequence<size>::type{}) {}
    // constexpr StringView(const char* str, Arrow::IntegerSequence<args...>) : data{at<args>(str)..., 0} {}

   constexpr StringView(charView<args>... argsChar) : data{argsChar..., 0} {}

   template <int... args1>
   constexpr StringView<args..., sizeof...(args) + args1...> operator+(StringView<args1...> b) const
   {
       return StringView<args..., sizeof...(args) + args1...>(at<args>(data)..., at<args1>(b.data)...);
    }

    // template <int... args1, int... args2>
    // constexpr StringView(const char* str1,
    //                      Arrow::IntegerSequence<args1...>,
    //                      const char* str2,
    //                      Arrow::IntegerSequence<args1...>) : data{at<args2>(str2)... ,0} {}

    // template<int size1>
    // StringView<size + size1> operator+(StringView<size1> b) const
    // {
    //     return StringView<size + size1>(this->data,
    //                                     typename Arrow::MakeIntegerSequence<size>::type{},
    //                                     b.data,
    //                                     typename Arrow::MakeIntegerSequence<size1>::type{});
    // }

    // template<int size1>
    // StringView<size + size1> operator+(StringView<size1> b) const
    // {
    //     return StringView<size + size1>(typename Arrow::MakeIntegerSequence<size + size1>::type{},
    //                                     typename Arrow::MakeIntegerSequence<size>::type{},
    //                                     b.data,
    //                                     typename Arrow::MakeIntegerSequence<size>::type{});
    // }
};


// template<int ...args>
// static auto StrViewToType(const StringView<sizeof...(args)>& sv) ->decltype(Arrow::ValueTypeList<char, at<args>(sv.data)...>)
// {
//     return Arrow::ValueTypeList<char, at<args>(sv.data)...>;
// }


template <int I>
struct get_string
{
    constexpr static auto g(const char* a) -> decltype(get_string<I - 1>::g(a) + StringView<0>(a + I))
    {
        return get_string<I - 1>::g(a) + StringView<0>(a + I);
    }
};

template <>
struct get_string<0>
{
    constexpr static StringView<0> g(const char* a)
    {
        return {a};
    }
};

template <int I>
constexpr auto str(const char (&a)[I]) -> decltype(get_string<I - 2>::g(a))
{
    return get_string<I - 2>::g(a);
}

template <char ch>
struct CharViewTest
{
};

// template <typename T>
// struct StrToTypeImpl;
// 
// template<int ...args>
// struct StrToTypeImpl<Arrow::IntegerSequence<args...>>
// {
// 
//     constexpr static auto __impl(charView<args>... argsChar)
//     {
//         return CharViewTest<argsChar...>{};
//     }
// 
//     template<int size>
//     constexpr static auto impl(const char(&a)[size]) //->   decltype(StrToTypeImpl<index - 1>::impl(std::forward(a)))
//     {
//         return __impl(at<args>(a)...);
//     }
// };
// 
// template<int size>
// constexpr auto StrToType(const char(&a)[size])// -> decltype(StrToTypeImpl<size - 1>::impl(a))
// {
//     return StrToTypeImpl<typename Arrow::MakeIntegerSequence<size>::type>::impl(a);
// }

template <int index>
struct StrToTypeImpl
{
    template<int size>
    constexpr static auto impl(const char(&a)[size]) //->   decltype(StrToTypeImpl<index - 1>::impl(std::forward(a)))
    {
        return CharViewTest<at1<index>(a)>{};
        // return Arrow::PushBack<Arrow::ValueType<char, at<index>(a)>, decltype(StrToTypeImpl<index - 1>::impl(std::forward(a))) >{};
    }
};
template <>
struct StrToTypeImpl<-1>
{
    template<int size>
    constexpr static Arrow::ValueTypeList<char, 0> impl(const char(&a)[size])
    {
        return Arrow::ValueTypeList<char, 0>{};
    }
};
template<int size>
constexpr auto StrToType(const char(&a)[size])// -> decltype(StrToTypeImpl<size - 1>::impl(a))
{
    return StrToTypeImpl<size - 1>::impl(a);
}

namespace StaticStr
{

namespace details
{

// template <int index>
// constexpr char at(const char* a) { return a[index]; }

template<typename T>
struct SubStrImpl;

template<int ...args>
struct SubStrImpl<Arrow::IntegerSequence<args...>>
{
    const StringView<sizeof...(args)> strView;
    constexpr SubStrImpl() : strView() {}
    constexpr SubStrImpl(const char* str) : strView(str) {}
    constexpr SubStrImpl(const char* a ,int nStart) : strView(a + nStart) {}

    
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





// template<int start, int length>
// constexpr StringView<length> SubStr(const char* sz)
// {
//     return StringView<Arrow::MakeIntegerSequence<length>::type>(sz + start);
// }



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