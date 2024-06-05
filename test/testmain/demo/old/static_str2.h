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

template <size_t index>
constexpr char at(const char* a) { return a[index]; }

template <size_t size>
struct StringView
{
    const char data[size + 1];
    const size_t length = 0;
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

    template <size_t size1>
    constexpr StringView<size + size1> operator+(StringView<size1> b) const
    {
       return StringView<size + size1>(typename Arrow::MakeIntegerSequence<size>::type{}, data,
                                       typename Arrow::MakeIntegerSequence<size1>::type{}, b.data);
    }
};

namespace details
{

struct StrLenImpl
{
    static constexpr size_t Impl(const char* sz, size_t nLength = 0)
    {
       return (*sz != 0) ? StrLenImpl::Impl(sz + 1, nLength + 1) : nLength;
    }
};

struct FindStrImplAssist
{

private:
    template<size_t N, int ...args>
    static constexpr bool Equal(const char* str1, const char (&str2)[N], Arrow::IntegerSequence<args...>)
    {
        // bool bRet = ((at<args>(str1) == at<args>(str2)) && ... && true);
        return ((at<args>(str1) == at<args>(str2)) && ... && true);
    }

public:
    template<size_t N>
    static constexpr bool Equal(const char* str1, const char (&str2)[N])
    {
        return StrLenImpl::Impl(str1, 0) < N - 1 ? false : Equal(str1, str2, typename Arrow::MakeIntegerSequence<N - 1>::type{});
    }



    // template<size_t N1, size_t N2>
    // static constexpr bool Equal(const char (&str1)[N1], const char (&str2)[N2])
    // {
    //     return N1 < N2 ? false : Equal(str1, str2, typename Arrow::MakeIntegerSequence<N2>::type{});
    // }

    // template<size_t N1, size_t N2, int ...args>
    // static constexpr bool Equal(const char (&str1)[N1], const char (&str2)[N2], Arrow::IntegerSequence<args...>)
    // {
    //     return ((at<args>(str1) == at<args>(str2)) && ... && true);
    // }
};


// 查找第 times 次出现的位置 0:查找最后一次出现的位置  1:查找第一次出现的位置[zhuyb 2023-06-21 22:31:48]
template<size_t times>
struct FindImpl
{
    static constexpr size_t Impl(const char* str, const char ch, size_t index = 0, size_t pos = -1)
    {
        return (str[index] == 0) ?  pos : ((str[index] == ch) ? 
                                            FindImpl<times - 1>::Impl(str, ch, index + 1, index) : 
                                            FindImpl<times>::Impl(str, ch, index + 1, pos));
    }

    template <size_t N>
    static size_t Impl(const char* str1, const char (&str2)[N], size_t index = 0, size_t pos = -1)
    {
        return (str1[index] == 0) ? pos : (FindStrImplAssist::Equal(str1 + index, str2) ? 
                                            FindImpl<times - 1>::Impl(str1, str2, index + 1, index) : 
                                            FindImpl<times>::Impl(str1, str2, index + 1, pos));
    }

};

template<>
struct FindImpl<1>
{
    static constexpr size_t Impl(const char* str, const char ch, size_t index = 0, size_t pos = -1)
    {
        return (str[index] == 0) ?  pos : ((str[index] == ch) ? 
                                            index : 
                                            FindImpl<1>::Impl(str, ch, index + 1, pos));
    }

    template<size_t N>
    static constexpr size_t Impl(const char* str1, const char (&str2)[N], size_t index = 0, size_t pos = -1)
    {
        return (str1[index] == 0) ? pos : (FindStrImplAssist::Equal(str1 + index, str2) ? 
                                            index : 
                                            FindImpl<1>::Impl(str1, str2, index + 1, pos));
    }

};

template<>
struct FindImpl<0>
{

    static constexpr size_t Impl(const char* str,const char ch, size_t index = 0, size_t pos = -1)
    {
        return (str[index] == 0) ? pos : ((str[index] == ch) ? 
                                            FindImpl<0>::Impl(str, ch, index + 1, index) : 
                                            FindImpl<0>::Impl(str, ch, index + 1, pos));
    }

    template<size_t N>
    static constexpr size_t Impl(const char* str1, const char (&str2)[N], size_t index = 0, size_t pos = -1)
    {
        return (str1[index] == 0) ? pos : (FindStrImplAssist::Equal(str1 + index, str2) ? 
                                        FindImpl<0>::Impl(str1, str2, index + 1, index) : 
                                        FindImpl<0>::Impl(str1, str2, index + 1, pos));
    }

};


template <size_t start, size_t length = size_t(-1)>
struct SubStrImpl
{
    template<size_t N>
    static constexpr StringView<length> Impl(const char (&sz)[N])
    {
       return StringView<length>(typename Arrow::MakeIntegerSequence<length>::type{}, sz + start);
    }
};

template <size_t start>
struct SubStrImpl<start, size_t(-1)>
{
    template<size_t N>
    static constexpr StringView<N - start - 1> Impl(const char (&sz)[N])
    {
       return StringView<N - start - 1>(typename Arrow::MakeIntegerSequence<N - start - 1>::type{}, sz + start);
    }
};


}

constexpr size_t StrLen(const char* sz)
{
    return details::StrLenImpl::Impl(sz);
}

template<size_t times = 1>
constexpr size_t Find(const char* sz, const char ch, int startPos = 0)
{
    return details::FindImpl<times>::Impl(sz, ch, startPos);
}

template<size_t times = 1, size_t N>
constexpr size_t Find(const char* str1, const char (&str2)[N], int startPos = 0)
{
    return details::FindImpl<times>::Impl(str1, str2, startPos);
}

template<size_t times, size_t size>
constexpr size_t Find(const StringView<size> sv,const  char ch, int startPos = 0)
{
    return Find<times>(sv.data, ch, startPos);
}

template<size_t times, size_t size, size_t N>
constexpr size_t Find(const StringView<size> sv, const char (&str)[N], int startPos = 0)
{
    return Find<times>(sv.data, str, startPos);
}

constexpr size_t FindLast(const char* sz,const char ch, int startPos = 0)
{
    return Find<0>(sz, ch, startPos);
}

template<size_t size>
constexpr size_t FindLast(const StringView<size> sv,const char ch, int startPos = 0)
{
    return Find<0>(sv.data, ch, startPos);
}

template<size_t N>
constexpr size_t FindLast(const char* sz, const char (&str)[N], int startPos = 0)
{
    return Find<0>(sz, str, startPos);
}

template<size_t size, size_t N>
constexpr size_t FindLast(const StringView<size> sv, const char (&str)[N], int startPos = 0)
{
    return Find<0>(sv.data, str, startPos);
}


template<size_t start, size_t length = size_t(-1), size_t N>
constexpr auto SubStr(const char (&sz)[N]) -> decltype(details::SubStrImpl<start, length>::Impl(sz))
{
    return details::SubStrImpl<start, length>::Impl(sz);
}

template<size_t start, size_t length = size_t(-1), size_t N>
constexpr auto SubStr(const StringView<N> sz) ->decltype(SubStr<start, length>(sz.data))
{
    return SubStr<start, length>(sz.data);
}

template<size_t length>
constexpr StringView<length - 1> Str(const char (&a)[length])
{
    return StringView<length - 1>(a);
}


template<typename T, int length = -1>
struct TypeName
{
    static void Trace()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        std::cout << __PRETTY_FUNCTION__ + Find<1>(__PRETTY_FUNCTION__, '=') << std::endl;
    }

    constexpr static StringView<length> Impl()
    {
        return StringView<length>(typename Arrow::MakeIntegerSequence<length>::type{},
                                 __PRETTY_FUNCTION__ + Find<1>(__PRETTY_FUNCTION__, "T = ") + StrLen("T = "));
    }
};

template<typename T>
struct TypeName<T, -1>
{
    static void Trace()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
        constexpr int start = Find<1>(__PRETTY_FUNCTION__, "T = ");
        std::cout << start << std::endl;
        std::cout << __PRETTY_FUNCTION__ + start << std::endl;
        constexpr int end = Find<1>(__PRETTY_FUNCTION__, ", length", start);
        std::cout << end << std::endl;
        std::cout << __PRETTY_FUNCTION__ + end << std::endl;
        std::cout << Find<0>(__PRETTY_FUNCTION__, ',') - Find<1>(__PRETTY_FUNCTION__, '=') - 2 << std::endl;
        TypeName<T, 0>::Cout();
    }

    constexpr static int Length()
    {
        return Find<1>(__PRETTY_FUNCTION__, ", length =") - Find<1>(__PRETTY_FUNCTION__, "T = ") - StrLen("T = ");
    }

    constexpr static auto Impl() -> decltype(TypeName<T, TypeName<T>::Length()>::Impl())
    {;
        return TypeName<T, TypeName<T>::Length()>::Impl();
    }

    constexpr static const char* Str()
    {
        return svName.data;
    }

    // static constexpr decltype(TypeName<T>::Impl()) svName = TypeName<T>::Impl();
    static constexpr decltype(TypeName<T>::Impl()) svName = TypeName<T>::Impl();
};
template<typename T>
constexpr decltype(TypeName<T>::Impl()) TypeName<T>::svName;


template<typename T, T t, int length = -1>
struct EnumItemName
{
    static void Trace()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    constexpr static StringView<length> Impl()
    {
        return StringView<length>(typename Arrow::MakeIntegerSequence<length>::type{},
                                  __PRETTY_FUNCTION__ + Find<1>(__PRETTY_FUNCTION__, "t = ") + StrLen("t = "));
    }
};

template<typename T, T t>
struct EnumItemName<T, t, -1>
{
    static void Trace()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;

        constexpr int start1 = Find<1>(__PRETTY_FUNCTION__, "T = ") + StrLen("T = ");
        constexpr int end1 = Find<1>(__PRETTY_FUNCTION__, ", t = ");
        constexpr int length1 = end1 - start1;
        constexpr auto svName1 = StringView<length1>(typename Arrow::MakeIntegerSequence<length1>::type{},
                                  __PRETTY_FUNCTION__ + start1);

        constexpr int start2 = Find<1>(__PRETTY_FUNCTION__, ", t = ") + StrLen(", t = ");
        constexpr int end2 = Find<1>(__PRETTY_FUNCTION__, ", length = ");
        constexpr int length2 = end2 - start2;
        constexpr auto svName2 = StringView<length1>(typename Arrow::MakeIntegerSequence<length2>::type{},
                                  __PRETTY_FUNCTION__ + start2);


        std::cout << svName1.data << std::endl;
        std::cout << svName2.data << std::endl;
        EnumItemName<T, t, 0>::Trace();
    }

    constexpr static int Length()
    {
        return Find<1>(__PRETTY_FUNCTION__, ", length = ") - Find<1>(__PRETTY_FUNCTION__, "t = ")  - StrLen("t = ");
    }
    
    /**
     * @description: 获取枚举值对应字符串
     * @return {*}
     */    
    constexpr static auto Impl() -> decltype(EnumItemName<T, t, EnumItemName<T, t>::Length()>::Impl())
    {
        return EnumItemName<T, t, EnumItemName<T, t>::Length()>::Impl();
    }

    constexpr static auto Impl1() -> decltype(TypeName<T>::Impl() + Str("::") + EnumItemName<T, t>::Impl())
    {
        return TypeName<T>::Impl() + Str("::") + EnumItemName<T, t>::Impl();
    }

    constexpr static const char* Name()
    {
        return value.data;
    }

    constexpr static const char* FullName()
    {
        return value1.data;
    }

    static constexpr decltype(EnumItemName<T, t>::Impl()) value = EnumItemName<T, t>::Impl();
    static constexpr decltype(EnumItemName<T, t>::Impl1()) value1 = EnumItemName<T, t>::Impl1();

};
template<typename T, T t>
constexpr decltype(EnumItemName<T, t>::Impl()) EnumItemName<T, t>::value;
template<typename T, T t>
constexpr decltype(EnumItemName<T, t>::Impl1()) EnumItemName<T, t>::value1;


}
}


enum MyEnum
{
    Em1A,
    Em2B,
    Em3C
};

static constexpr auto ss1 = ArrowTest::StaticStr::StringView<3>("123");
static constexpr auto ss2 = ArrowTest::StaticStr::StringView<4>("4567");
static void TestStrView2()
{


    // ArrowTest::StaticStr::TypeName<std::map<int, std::string>>::Cout();
    // constexpr auto s0 = ArrowTest::StaticStr::TypeName<std::map<int, std::string>>::Name();
    // std::cout << s0.data << std::endl;

    std::cout << ArrowTest::StaticStr::TypeName<std::map<int, std::string>>::svName.data << std::endl;
    std::cout << ArrowTest::StaticStr::TypeName<std::vector<int>>::svName.data << std::endl;
    std::cout << ArrowTest::StaticStr::TypeName<std::map<int, float>>::Str() << std::endl;

    ArrowTest::StaticStr::EnumItemName<MyEnum, Em1A>::Trace();
    std::cout << ArrowTest::StaticStr::EnumItemName<MyEnum, Em1A>::Name() << std::endl;
    std::cout << ArrowTest::StaticStr::EnumItemName<MyEnum, Em1A>::FullName() << std::endl;
    // std::cout << sA.data << std::endl;




    // Arrow::StaticStr::EnumItemName<MyEnum, Em1A>();

    // constexpr auto s0 = Arrow::StaticStr::TypeName<std::map<int, std::string>>();
    // std::cout << s0.data << std::endl;

    // constexpr auto sItem = Arrow::StaticStr::EnumItemName<MyEnum, Em1A>();
    // std::cout << sItem.data << std::endl;

    constexpr auto s1 = ArrowTest::StaticStr::Str("123");
    constexpr auto s2 = ArrowTest::StaticStr::Str("4567");
    constexpr auto s3 = s1 + s2 + s1;
    constexpr auto start = ArrowTest::StaticStr::Find<1>(s3, '2');
    constexpr auto s4 = ArrowTest::StaticStr::SubStr<start>(s3.data);
    constexpr auto s5 = ArrowTest::StaticStr::SubStr<2>(s3);
    std::cout << ArrowTest::StaticStr::Find(s3.data, "23") << std::endl;
    std::cout << ArrowTest::StaticStr::Find(s3.data, "34") << std::endl;
    std::cout << ArrowTest::StaticStr::Find<2>(s3.data, "23") << std::endl;

    std::cout << ArrowTest::StaticStr::StrLen(s1.data) << ":" << s1.data << std::endl;
    std::cout << ArrowTest::StaticStr::StrLen(s2.data) << ":" << s2.data << std::endl;
    std::cout << ArrowTest::StaticStr::StrLen(s3.data) << ":" << s3.data << std::endl;
    std::cout << ArrowTest::StaticStr::StrLen(s4.data) << ":" << s4.data << std::endl;
    std::cout << ArrowTest::StaticStr::StrLen(s5.data) << ":" << s5.data << std::endl;
    // constexpr auto s3 = s1+s2;
}