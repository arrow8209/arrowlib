#pragma once
#include <string>

namespace Arrow
{
namespace Other
{
template <typename T>
struct StrConvertImpl;

template<>
struct StrConvertImpl<signed char>
{
    template<typename StringType>
    static signed char Convert(const StringType& str, size_t* idx = 0, int base = 10)
    {
        return static_cast<signed char>(std::stoi(str, idx, base));
    }
};

template<>
struct StrConvertImpl<unsigned char>
{
    template<typename StringType>
    static unsigned char Convert(const StringType& str, size_t* idx = 0, int base = 10)
    {
        return static_cast<unsigned char>(std::stoul(str, idx, base));
    }
};

template<>
struct StrConvertImpl<int>
{
    template<typename StringType>
    static int Convert(const StringType& str, size_t* idx = 0, int base = 10)
    {
        return std::stoi(str, idx, base);
    }
};

template<>
struct StrConvertImpl<bool>
{
    template<typename StringType>
    static bool Convert(const StringType& str, size_t* idx = 0, int base = 10)
    {
        return static_cast<bool>(std::stoi(str, idx, base));
    }
};

template<>
struct StrConvertImpl<unsigned int>
{
    template<typename StringType>
    static unsigned int Convert(const StringType& str, size_t* idx = 0, int base = 10)
    {
        return static_cast<unsigned int>(std::stoul(str, idx, base));
    }
};

template<>
struct StrConvertImpl<long>
{
    template<typename StringType>
    static long Convert(const StringType& str, size_t* idx = 0, int base = 10)
    {
        return std::stol(str, idx, base);
    }
};

template<>
struct StrConvertImpl<unsigned long>
{
    template<typename StringType>
    static unsigned long Convert(const StringType& str, size_t* idx = 0, int base = 10)
    {
        return std::stoul(str, idx, base);
    }
};

template<>
struct StrConvertImpl<long long>
{
    template<typename StringType>
    static long long Convert(const StringType& str, size_t* idx = 0, int base = 10)
    {
        return std::stoul(str, idx, base);
    }
};

template<>
struct StrConvertImpl<unsigned long long>
{
    template<typename StringType>
    static unsigned long long Convert(const StringType& str, size_t* idx = 0, int base = 10)
    {
        return std::stoull(str, idx, base);
    }
};

template<>
struct StrConvertImpl<float>
{
    template<typename StringType>
    static float Convert(const StringType& str, size_t* idx = 0, int base = 10)
    {
        return std::stof(str, idx);
    }
};

template<>
struct StrConvertImpl<double>
{
    template<typename StringType>
    static double Convert(const StringType& str, size_t* idx = 0, int base = 10)
    {
        return std::stod(str, idx);
    }
};

template<>
struct StrConvertImpl<long double>
{
    template<typename StringType>
    static long double Convert(const StringType& str, size_t* idx = 0, int base = 10)
    {
        return std::stold(str, idx);
    }
};

template<>
struct StrConvertImpl<void*>
{
    template<typename StringType>
    static void* Convert(const StringType& str, size_t* idx = 0, int base = 10)
    {
        return reinterpret_cast<void*>(std::stoul(str, idx));
    }
};

template<typename T>
T StrConvert(const std::string& str, size_t* idx = 0, int base = 10)
{
    return StrConvertImpl<T>::Convert(str, idx, base);
}

template<typename T>
T StrConvert(const std::wstring& str, size_t* idx = 0, int base = 10)
{
    return StrConvertImpl<T>::Convert(str, idx, base);
}

}
} // namespace Arrow