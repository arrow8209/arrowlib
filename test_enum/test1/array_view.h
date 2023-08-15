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
    constexpr ArrayView() : data{static_cast<T>(0)}, length(size) {}

    constexpr ArrayView(T val) : data{val}, length(size) {}

    constexpr ArrayView(const T (&array)[size]): data{array[args]...}, length(size) {}

    // constexpr ArrayView(TDummy<args>... vals) : data{vals...}, length(size) {}

    constexpr ArrayView(const T* array) : data{get<T, args>(array)...}, length(sizeof...(args)) {}

    template <int... args1, int... args2>
    constexpr ArrayView(const T* array1, Arrow::IntegerSequence<args1...>, const T* array2, Arrow::IntegerSequence<args2...>)
        : data{get<T, args1>(array1)..., get<T, args2>(array2)...}, length(sizeof...(args1) + sizeof...(args2)) {}

    template <size_t size1>
    constexpr ArrayView<T, size + size1> operator+(const ArrayView<T, size1>& b) const
    {
        return ArrayView<T, size + size1>(data, typename Arrow::MakeIntegerSequence<size>::type{},
                                        b.data, typename Arrow::MakeIntegerSequence<size1>::type{});
    }

    constexpr ArrayView<T, size> operator+(const ArrayView<T, 0>& b)
    {
        return *this;
    }

    constexpr T operator[](size_t i) const
    {
        return data[i];
    }

    T& operator[](size_t i)
    {
        return data[i];
    }

};

template<typename T, int... argsMain>
struct ArrayView<T, 0, Arrow::IntegerSequence<argsMain...>>
{
    template <size_t size1>
    constexpr ArrayView<T, size1> operator+(const ArrayView<T, size1>& b) const
    {
        return ArrayView<T, size1>(b.data, typename Arrow::MakeIntegerSequence<size1>::type{});
    }

    constexpr ArrayView<T, 0> operator+(const ArrayView<T, 0>& b) const
    {
        return *this;
    }
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
