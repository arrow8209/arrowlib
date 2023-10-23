#pragma once

#include <tuple>
#include "../typelist/typelist.h"

namespace Arrow
{

namespace Other
{

namespace details
{

template <typename Sequence, typename ...Args>
struct SetTupleImpl
{
};

template<int first, int ...I, typename T, typename ...Args>
struct SetTupleImpl<Arrow::IntegerSequence<first, I...>, T, Args...>
{
    template<typename tupleType>
    inline static void Impl(tupleType& tuple, T val, Args ...args)
    {
        std::get<first>(tuple) = val;
        SetTupleImpl<Arrow::IntegerSequence<I...>, Args...>::Impl(tuple, args...);
    }
};

template<int end, typename T>
struct SetTupleImpl<Arrow::IntegerSequence<end>, T>
{
    template <typename tupleType>
    inline static void Impl(tupleType& tuple, T val)
    {
        std::get<end>(tuple) = val;

    }
};
}

// template <std::size_t I = 0, typename... Tp>
// inline typename std::enable_if<I == sizeof...(Tp), void>::type
// assign_from_tuple(const std::tuple<Tp...>& t, Tp&... args)
// {
//     // base case: do nothing
// }

// template <std::size_t I = 0, typename... Tp>
//     inline typename std::enable_if < I<sizeof...(Tp), void>::type
//                                      assign_from_tuple(const std::tuple<Tp...>& t, Tp&... args)
// {
//     std::get<I>(t) = std::get<I>(args...);
//     assign_from_tuple<I + 1, Tp...>(t, args...);
// }

template<typename ...Args>
inline static void SetTuple(std::tuple<Args...>& tuple, Args ...args)
{
    details::SetTupleImpl<typename Arrow::MakeIntegerSequence<sizeof...(Args)>::type, Args...>::Impl(tuple, args...);
}

}
}