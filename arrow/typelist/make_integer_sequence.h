#pragma once

namespace Arrow
{

template <int... args>
struct IntegerSequence
{
};

namespace details
{

template<int... seq>
struct MakeIntegerSequenceImpl;

template<int start, int... seq>
struct MakeIntegerSequenceImpl<start, seq...>
{
  using type = typename MakeIntegerSequenceImpl<start - 1, start - 1, seq...>::type;
};

template<int... seq>
struct MakeIntegerSequenceImpl<0, seq...>
{
  using type = IntegerSequence<seq...>;
};

}

template<int count>
using MakeIntegerSequence = typename details::MakeIntegerSequenceImpl<count>;

}