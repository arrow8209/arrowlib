#pragma once

namespace Arrow
{

template <int... args>
struct IntegerSequence
{
};

namespace details
{

template<size_t... seq>
struct MakeIntegerSequenceImpl;

template<size_t start, size_t... seq>
struct MakeIntegerSequenceImpl<start, seq...>
{
  using type = typename MakeIntegerSequenceImpl<start - 1, start - 1, seq...>::type;
};

template<size_t... seq>
struct MakeIntegerSequenceImpl<0, seq...>
{
  using type = IntegerSequence<seq...>;
};

}

template<size_t count>
using MakeIntegerSequence = typename details::MakeIntegerSequenceImpl<count>;

}