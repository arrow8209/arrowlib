/*
 * @FilePath: arrow/other/static_const.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2023-01-14 14:18:05
 * @Description: 弃用，危险性太高，与代理的收益不成比例，主要是在不同的人定义的相同的类型，会产生不可预知的问题
 */
#pragma once

namespace Arrow
{

namespace Other
{


// template<typename T>
// struct StaticConst
// {
//     static constexpr T value{};
// };

// template<typename T>
// constexpr T StaticConst<T>::value; // NOLINT(readability-redundant-declaration)

// template<typename T>
// struct StaticValue
// {
//     static T value;
// };

// template<typename T>
// T StaticValue<T>::value{};

}
}