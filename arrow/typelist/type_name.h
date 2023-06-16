#pragma once
#include <type_traits>
#include "typelist_type.h"
#include "make_integer_sequence.h"

namespace Arrow
{

template<typename T>
struct TypeName
{
    static std::string Name()
    {
        return typeid(T).name();
    }
};

template<>
struct TypeName<TypeList<>>
{
    static std::string Name()
    {
        static std::string strName = "TypeList<>";
        return strName;
    }
};

template<typename T, typename ...Args>
struct TypeName<TypeList<T, Args...>>
{
    static std::string Name()
    {
        std::string result = "TypeList<" + TypeName<T>::Name();
        ((result += ", " + TypeName<Args>::Name()), ...);
        result += ">";
        return result;
    }
};

template<typename T, T t>
struct TypeName<ValueType<T, t>>
{
    static std::string Name()
    {
        std::string result = "ValueType<" + TypeName<T>::Name() + ":" + std::to_string(t) + ">";
        return result;
    }
};

template<>
struct TypeName<IntegerSequence<>>
{
    static std::string Name()
    {
        std::string result = "IntegerSequence<>";
        return result;
    }
};

template<int n, int ...args>
struct TypeName<IntegerSequence<n, args...>>
{
    static std::string Name()
    {
        std::string result = "IntegerSequence<" + std::to_string(n);
        ((result += ", " +  std::to_string(args)), ...);
        result += ">";
        return result;
    }
};

}