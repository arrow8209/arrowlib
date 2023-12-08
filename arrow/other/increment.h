#pragma once
#include <limits>
#include <atomic>

namespace Arrow
{

namespace Other
{
template <typename T,
          typename _ValueType = uint32_t,
          _ValueType _minValue = std::numeric_limits<_ValueType>::min(),
          _ValueType _maxValue = std::numeric_limits<_ValueType>::max()>
struct TIncrement
{
    typedef _ValueType ValueType;
    constexpr static ValueType minValue = _minValue;
    constexpr static ValueType maxValue = _maxValue;
    static std::atomic<ValueType> s_value;

    static ValueType Get()
    {
        ValueType oldValue = s_value.fetch_add(1);
        if(oldValue >= maxValue)
        {
            s_value.store(minValue);
        }
        return oldValue;
    }

};
template <typename T, typename _ValueType, _ValueType _minValue, _ValueType _maxValue>
std::atomic<_ValueType> TIncrement<T, _ValueType, _minValue, _maxValue>::s_value{_minValue};
}
}