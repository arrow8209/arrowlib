#pragma once
#include <atomic>
#include <limits>

namespace Arrow
{

namespace Other
{
template <typename T, typename ValueType = int32_t>
struct TypeCount
{
    static std::atomic<ValueType> gValue;

    static ValueType Add()
    {
        return gValue.fetch_add(1);
    }

    static ValueType Sub()
    {
        return gValue.fetch_sub(1);
    }

    static ValueType Value()
    {
        return gValue.load();
    }
};
template <typename T, typename ValueType>
std::atomic<ValueType> TypeCount<T, ValueType>::gValue{0};
} // namespace Other
} // namespace Arrow