#pragma once
#include <stdint.h>
#include <map>
#include <string>
#include "arrow/typelist/type_list_fun/type_to_data.h"
#include "arrow/typelist/static_string.h"
#include "arrow/typelist/make_integer_sequence.h"

namespace ArrowTest
{

namespace StaticStr
{

template <int index>
constexpr char at(const char* a) { return a[index]; }

template <int size>
struct StringView
{
    const char data[size + 1];
    const int length = 0;
    constexpr StringView() : data{}, length(0) {}

    constexpr StringView(const char* str) : StringView(typename Arrow::MakeIntegerSequence<size>::type{}, str) {}

    template <int... args>
    constexpr StringView(Arrow::IntegerSequence<args...>, const char* str) : data{at<args>(str)..., 0}, length(sizeof...(args)){}

    // template<int ...args>
    // constexpr StringView(Arrow::IntegerSequence<args...>, charView<args>... argsChar) : data{argsChar..., 0} {}
    // template <int size1, int size2>
    // constexpr StringView(StringView<size1> sv1, StringView<size2> sv2) 
    //    : StringView(sv1, typename Arrow::MakeIntegerSequence<size2>::type{}, sv2.data) {}
    // template <int size1, int ...args>
    // constexpr StringView(StringView<size1> sv1, Arrow::IntegerSequence<args...>,const char* sz2)
    //     : StringView(typename Arrow::MakeIntegerSequence<size1>::type{}, sv1.data, at<args>(sz2)...) {}
    // template <int ...args, typename ...CHS>
    // constexpr StringView(Arrow::IntegerSequence<args...>, const char* sz1, CHS ...chs )
    //     : data{at<args>(sz1)..., chs..., 0} {}
    // template <int size1, int size2>
    // constexpr StringView(StringView<size1> sv1, StringView<size2> sv2) 
    //    : StringView(typename Arrow::MakeIntegerSequence<size1>::type{}, sv1.data, typename Arrow::MakeIntegerSequence<size2>::type{}, sv2.data) {}

    template <int... args1, int... args2>
    constexpr StringView(Arrow::IntegerSequence<args1...>, const char* sz1, Arrow::IntegerSequence<args2...>, const char* sz2)
        : data{at<args1>(sz1)..., at<args2>(sz2)..., 0}, length(sizeof...(args1) + sizeof...(args2)) {}

    template <int size1>
    constexpr StringView<size + size1> operator+(StringView<size1> b) const
    {
       return StringView<size + size1>(typename Arrow::MakeIntegerSequence<size>::type{}, data,
                                       typename Arrow::MakeIntegerSequence<size1>::type{}, b.data);
    }

};

namespace details
{

// 查找第 times 次出现的位置 0:查找最后一次出现的位置  1:查找第一次出现的位置[zhuyb 2023-06-21 22:31:48]
template<int times>
struct FindImpl
{
    static constexpr int Impl(const char* str, char ch, int index, int pos)
    {
        return (*str == 0) ?  pos : ((*str == ch) ? FindImpl<times - 1>::Impl(str + 1, ch, index + 1, index) : FindImpl<times>::Impl(str + 1, ch, index + 1, pos));
    }
};

template<>
struct FindImpl<1>
{
    static constexpr int Impl(const char* str, char ch, int index, int pos)
    {
        return (*str == 0) ?  pos : ((*str == ch) ? index : FindImpl<1>::Impl(str + 1, ch, index + 1, pos));
    }
};

template<>
struct FindImpl<0>
{
    static constexpr int Impl(const char* str, char ch, int index, int pos)
    {
        return (*str == 0) ? pos : ((*str == ch) ? FindImpl<0>::Impl(str + 1, ch, index + 1, index) : FindImpl<0>::Impl(str + 1, ch, index + 1, pos));
    }
};

constexpr int StrLenImpl(const char* sz, int nLength)
{
    return (*sz != 0) ? StrLenImpl(sz + 1, nLength + 1) : nLength;
}

template <int start, int length>
struct SubStrImpl
{
    static constexpr StringView<length> Impl(const char* sz)
    {
       return StringView<length>(typename Arrow::MakeIntegerSequence<length>::type{}, sz + start);
    }
};

// gcc 版本 [zhuyb 2023-06-19 17:25:03]
template<int length>
constexpr auto TypeNameImpl(const char (&sz)[length]) ->decltype(SubStrImpl<FindImpl(sz, '=', 0) + 2, length - 1 - FindImpl(sz, '=', 0) - 2 - 1>::Impl(sz))
{
    return SubStrImpl<FindImpl(sz, '=', 0) + 2, length - 1 - FindImpl(sz, '=', 0) - 2 - 1>::Impl(sz);
}

}

template<int times = 1>
constexpr int Find(const char* sz, char ch)
{
    return details::FindImpl<times>::Impl(sz, ch, 0, -1);
}

template<int times, int length>
constexpr int Find(const StringView<length> sv, char ch)
{
    return Find<times>(sv.data, ch);
}

constexpr int FindLast(const char* sz, char ch)
{
    return Find<0>(sz, ch);
}

template<int length>
constexpr int FindLast(const StringView<length> sv, char ch)
{
    return Find<0>(sv.data, ch);
}


constexpr int StrLen(const char* sz)
{
    return details::StrLenImpl(sz, 0);
}

template<int start, int length, int szSize>
constexpr auto SubStr(const char (&sz)[szSize]) -> decltype(details::SubStrImpl<start, length>::Impl(sz))
{
    return details::SubStrImpl<start, length>::Impl(sz);
}

template<int start, int szSize>
constexpr auto SubStr(const char (&sz)[szSize]) -> decltype(details::SubStrImpl<start, szSize - start>::Impl(sz))
{
    return details::SubStrImpl<start, szSize - start>::Impl(sz);
}

template<int start, int length, int szSize>
constexpr auto SubStr(const StringView<szSize> sz) ->decltype(SubStr<start, length>(sz.data))
{
    return SubStr<start, length>(sz.data);
}

template<int start, int svSize>
constexpr auto SubStr(const StringView<svSize> sv) ->decltype(SubStr<start, svSize-start>(sv.data))
{
    return SubStr<start, svSize-start>(sv.data);
}

template<int length>
constexpr StringView<length - 1> Str(const char (&a)[length])
{
    return StringView<length - 1>(a);
}

template<typename T, int length = -1>
struct TypeName
{
    static void Cout()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::cout << __PRETTY_FUNCTION__ + Find<1>(__PRETTY_FUNCTION__, '=') << std::endl;
    }

    constexpr static StringView<length> Name()
    {
        return StringView<length>(typename Arrow::MakeIntegerSequence<length>::type{},
                                 __PRETTY_FUNCTION__ + Find<1>(__PRETTY_FUNCTION__, '=') + 2);
    }
};

template<typename T>
struct TypeName<T, -1>
{
    static void Cout()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::cout << Find<0>(__PRETTY_FUNCTION__, ',') << std::endl;
        std::cout << __PRETTY_FUNCTION__ + Find<0>(__PRETTY_FUNCTION__, ',') << std::endl;
        std::cout << Find<1>(__PRETTY_FUNCTION__, 's') << std::endl;
        std::cout << __PRETTY_FUNCTION__ + Find<1>(__PRETTY_FUNCTION__, 's') << std::endl;
        std::cout << Find<0>(__PRETTY_FUNCTION__, ',') - Find<1>(__PRETTY_FUNCTION__, '=') - 2 << std::endl;
        TypeName<T, 0>::Cout();
    }

    constexpr static int Length()
    {
        return Find<0>(__PRETTY_FUNCTION__, ',') - Find<1>(__PRETTY_FUNCTION__, '=') - 2;
    }

    constexpr static auto Name() -> decltype(TypeName<T, TypeName<T>::Length()>::Name())
    {;
        return TypeName<T, TypeName<T>::Length()>::Name();
    }
};


template<typename T, T t, int length = -1>
struct EnumItemName
{
    static void Cout()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    static StringView<length> Name()
    {
        return StringView<length>(typename Arrow::MakeIntegerSequence<length>::type{},
                                  __PRETTY_FUNCTION__ + Find<2>(__PRETTY_FUNCTION__, '=') + 2);
    }
};

template<typename T, T t>
struct EnumItemName<T, t, -1>
{
    static void Cout()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::cout << Find<0>(__PRETTY_FUNCTION__, ',') << std::endl;
        std::cout << Find<2>(__PRETTY_FUNCTION__, '=')  << std::endl;
        std::cout << __PRETTY_FUNCTION__ + Find<2>(__PRETTY_FUNCTION__, '=') << std::endl;
        std::cout << __PRETTY_FUNCTION__ + Find<0>(__PRETTY_FUNCTION__, ',') << std::endl;
        std::cout << Find<0>(__PRETTY_FUNCTION__, ',') - Find<2>(__PRETTY_FUNCTION__ , '=') - 2 << std::endl;
        EnumItemName<T, t, 0>::Cout();
    }

    constexpr static int Length()
    {
        return Find<0>(__PRETTY_FUNCTION__, ',') - Find<2>(__PRETTY_FUNCTION__ , '=') - 2;
    }
    

    static auto Name() -> decltype(EnumItemName<T, t, EnumItemName<T, t>::Length()>::Name())
    {
        return EnumItemName<T, t, EnumItemName<T, t>::Length()>::Name();
    }
};

// template<typename T, T t>
// constexpr auto EnumItemName()
// {
//     return SubStr<FindLast(__PRETTY_FUNCTION__, '=') + 2, 
//                     StrLen(__PRETTY_FUNCTION__) - FindLast(__PRETTY_FUNCTION__, '=') - 2 - 1>(__PRETTY_FUNCTION__);
// }


}
}


enum MyEnum
{
    Em1A,
    Em2B,
    Em3C
};

static void TestStrView2()
{
    // constexpr auto s1 = Arrow::StringView<3>("123");
    // constexpr auto s2 = Arrow::StringView<4>("4567");
    // constexpr auto s0 = ArrowTest::StaticStr::TypeName<std::map<int, std::string>>();
    // std::cout << s0.data << std::endl;

    // std::cout << ArrowTest::StaticStr::Find<2>("01234567890123", '0') << std::endl;
    // std::cout << "01234567890123" + ArrowTest::StaticStr::Find<0>("01234567890123", '0') << std::endl;


    ArrowTest::StaticStr::TypeName<std::map<int, std::string>>::Cout();
    constexpr auto s0 = ArrowTest::StaticStr::TypeName<std::map<int, std::string>>::Name();
    std::cout << s0.data << std::endl;

    ArrowTest::StaticStr::EnumItemName<MyEnum, Em1A>::Cout();
    auto sA = ArrowTest::StaticStr::EnumItemName<MyEnum, Em1A>::Name();
    std::cout << sA.data << std::endl;




    // Arrow::StaticStr::EnumItemName<MyEnum, Em1A>();

    // constexpr auto s0 = Arrow::StaticStr::TypeName<std::map<int, std::string>>();
    // std::cout << s0.data << std::endl;

    // constexpr auto sItem = Arrow::StaticStr::EnumItemName<MyEnum, Em1A>();
    // std::cout << sItem.data << std::endl;

    constexpr auto s1 = ArrowTest::StaticStr::Str("123");
    constexpr auto s2 = ArrowTest::StaticStr::Str("4567");
    constexpr auto s3 = s1 + s2;
    constexpr auto start = ArrowTest::StaticStr::Find<1>(s3, '2');
    constexpr auto s4 = ArrowTest::StaticStr::SubStr<start>(s3.data);
    constexpr auto s5 = ArrowTest::StaticStr::SubStr<1>(s3);
    std::cout << ArrowTest::StaticStr::StrLen(s1.data) << ":" << s1.data << std::endl;
    std::cout << ArrowTest::StaticStr::StrLen(s2.data) << ":" << s2.data << std::endl;
    std::cout << ArrowTest::StaticStr::StrLen(s3.data) << ":" << s3.data << std::endl;
    std::cout << ArrowTest::StaticStr::StrLen(s4.data) << ":" << s4.data << std::endl;
    std::cout << ArrowTest::StaticStr::StrLen(s5.data) << ":" << s5.data << std::endl;
    // constexpr auto s3 = s1+s2;
}