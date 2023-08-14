#pragma once
#include "arrow/typelist/make_integer_sequence.h"
#include <map>
#include <stdint.h>
#include <string>

namespace Arrow2
{

namespace Array
{

template <typename T, size_t index>
constexpr T get(const T* a) { return a[index]; }

template<typename T, std::size_t M, std::size_t N>
constexpr const T get(const T (&arr)[N])
{
    return arr[ M < N ? M : N-1];
}

template <typename T, size_t size, typename Sequence=typename Arrow::MakeIntegerSequence<size>::type>
struct ArrayView
{};

template <typename T, size_t size, int... args>
struct ArrayView<T, size, Arrow::IntegerSequence<args...>>
{
    template<size_t n>
    using TDummy = T;

    const T data[size];
    const size_t length = 0;
    constexpr ArrayView() : data{0}, length(0) {}

    constexpr ArrayView(const T (&array)[size]): data{array[args]...}, length(size) {}

    constexpr ArrayView(TDummy<args>... vals) : data{vals...}, length(size) {}

    constexpr ArrayView(const T* array) : data{get<T, args>(array)...}, length(sizeof...(args)) {}

    template <int... args1, int... args2>
    constexpr ArrayView(const T* array1, Arrow::IntegerSequence<args1...>, const T* array2, Arrow::IntegerSequence<args2...>)
        : data{get<T, args1>(array1)..., get<T, args2>(array2)...}, length(sizeof...(args1) + sizeof...(args2)) {}

    template <size_t size1>
    constexpr ArrayView<T, size + size1> operator+(ArrayView<T, size1> b) const
    {
        return ArrayView<T, size + size1>(data, typename Arrow::MakeIntegerSequence<size>::type{},
                                        b.data, typename Arrow::MakeIntegerSequence<size1>::type{});
    }

    constexpr ArrayView<T, size> operator+(ArrayView<T, 0> b) const
    {
        return *this;
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

template<typename T, int... argsMain>
struct ArrayView<T, 0, Arrow::IntegerSequence<argsMain...>>
{
};

namespace details
{

// struct ArrayViewLenImpl
// {
//     static constexpr size_t Impl(const char* sz, size_t nLength = 0)
//     {
//        return (*sz != 0) ? StrLenImpl::Impl(sz + 1, nLength + 1) : nLength;
//     }
// };
}

// template<typename T, size_t length>
// constexpr ArrayView<T, length> Array(const T (&a)[length])
// {
//     return StringView<length - 1>(a);
// }


}
}
