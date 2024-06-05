#pragma once

namespace nlohmann
{
template<typename _T>
struct adl_serializer<_T*, void>
{
    // template<typename BasicJsonType, typename TargetType = _T>
    // static void from_json(BasicJsonType && j, TargetType*& val) 
    // {
    //     nlohmann::from_json(std::forward<BasicJsonType>(j), val);
    // }

    // template<typename BasicJsonType, typename TargetType = _T>
    // static void to_json(BasicJsonType& j, TargetType* val) 
    // {
    //    nlohmann::to_json(j, std::forward<TargetType*>(val));
    // }


    template<typename BasicJsonType, typename TargetType = _T>
    static auto from_json(BasicJsonType && j, TargetType*& val) noexcept(
        noexcept(::nlohmann::from_json(std::forward<BasicJsonType>(j), val)))
    -> decltype(::nlohmann::from_json(std::forward<BasicJsonType>(j), val), void())
    {
        ::nlohmann::from_json(std::forward<BasicJsonType>(j), val);
    }

    /// @brief convert any value type to a JSON value
    /// @sa https://json.nlohmann.me/api/adl_serializer/to_json/
    template<typename BasicJsonType, typename TargetType = _T>
    static auto to_json(BasicJsonType& j, TargetType* val) noexcept(
        noexcept(::nlohmann::to_json(j, std::forward<TargetType*>(val))))
    -> decltype(::nlohmann::to_json(j, std::forward<TargetType*>(val)), void())
    {
        ::nlohmann::to_json(j, std::forward<TargetType*>(val));
    }

};

//  template <
//         typename T, std::size_t N,
//         typename Array = T (&)[N],

template<size_t N>
struct adl_serializer<char(&)[N], void>
{
    template<typename BasicJsonType>
    static void from_json(BasicJsonType && j, char (&val)[N]) 
    {
        // nlohmann::from_json(std::forward<BasicJsonType>(j), val);
        typedef typename std::remove_reference<typename std::remove_cv<BasicJsonType>::type>::type::string_t string_t;
        const string_t* pStr = j.template get_ptr<const string_t*>();

        if (pStr->size() < N)
        {
            // max_memcpy_s(val, N - 1, pStr->c_str(), pStr->size() + 1);
            strcpy(val, pStr->c_str());
        }
        else
        {
            // max_memcpy_s(val, N - 1, pStr->c_str(), N - 1);
            memcpy(val, pStr->c_str(), N - 1);
            val[N-1] = 0;
        }
        
    }

    template<typename BasicJsonType>
    static void to_json(BasicJsonType& j,const char (&val)[N])
    {
       nlohmann::to_json(j, std::forward<char(&)[N]>(val));
    }


    // template<typename BasicJsonType>
    // static auto from_json(BasicJsonType && j, char[_N]& val) noexcept(
    //     noexcept(::nlohmann::from_json(std::forward<BasicJsonType>(j), val)))
    // -> decltype(::nlohmann::from_json(std::forward<BasicJsonType>(j), val), void())
    // {
    //     ::nlohmann::from_json(std::forward<BasicJsonType>(j), val);
    // }

    // /// @brief convert any value type to a JSON value
    // /// @sa https://json.nlohmann.me/api/adl_serializer/to_json/
    // template<typename BasicJsonType>
    // static auto to_json(BasicJsonType& j, char[_N]& val) noexcept(
    //     noexcept(::nlohmann::to_json(j, std::forward<char[_N]>(val))))
    // -> decltype(::nlohmann::to_json(j, std::forward<char[_N]>(val)), void())
    // {
    //     ::nlohmann::to_json(j, std::forward<char[_N]>(val));
    // }

};

}