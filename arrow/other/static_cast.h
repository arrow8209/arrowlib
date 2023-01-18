#pragma once

namespace Arrow
{

namespace Other
{


template<typename T>
struct StaticConst
{
    static constexpr T value{};
};

template<typename T>
constexpr T StaticConst<T>::value; // NOLINT(readability-redundant-declaration)

}
}