#pragma once

namespace Arrow2
{
using namespace Arrow;

template <typename T, T val, typename EndTag=void>
struct EnumNameAssist
{
    static size_t TraceLength()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        return 0;
    }
    static constexpr size_t Length()
    {
        return StaticStr::Find<1>(__PRETTY_FUNCTION__, " EndTag = ") - 1 - StaticStr::Find<1>(__PRETTY_FUNCTION__, " val = ") - StaticStr::StrLen(" val = ");
    }

    static constexpr auto View() -> StaticStr::StringView<Length()>
    {   
        return StaticStr::StringView<Length()>(__PRETTY_FUNCTION__ + StaticStr::Find<1>(__PRETTY_FUNCTION__, " val = ") + StaticStr::StrLen(" val = "));
    }
    static constexpr decltype(EnumNameAssist<T, val, EndTag>::View()) value = EnumNameAssist<T, val, EndTag>::View();
    static constexpr size_t length = Length();
};
template <typename T, T val, typename EndTag>
constexpr decltype(EnumNameAssist<T, val, EndTag>::View()) EnumNameAssist<T, val, EndTag>::value;
template<typename T, T val, typename EndTag>
constexpr size_t EnumNameAssist<T, val, EndTag>::length;

}
