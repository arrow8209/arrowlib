#pragma once
#include <vector>
#include <string>
#include "../arrow/arrow.h"

static void TestStaticMap()
{
    typedef  Arrow::static_map<
     Arrow::static_pair<Arrow::IntValueType<1>,STATIC_STRING("1234")>,
     Arrow::static_pair<Arrow::IntValueType<3>,STATIC_STRING("2345")>,
     Arrow::static_pair<Arrow::IntValueType<5>,STATIC_STRING("3456")>,
     Arrow::static_pair<Arrow::IntValueType<7>,STATIC_STRING("4567")>
     > SMap1;
    
    std::cout << Arrow::TypeName<SMap1>::Name() << std::endl;
    //  Arrow::print(SMap1{});

     typedef Arrow::static_map<> SMap2;

    typedef Arrow::smap::insert<SMap2, Arrow::static_pair<Arrow::IntValueType<8>,STATIC_STRING("4567")>>::type SMap3;
    std::cout << Arrow::TypeName<SMap3>::Name() << std::endl;
    // Arrow::print(SMap3{});
     
    //  Arrow::print(typename Arrow::smap::get<Arrow::IntValueType<7>, SMap1>::Pair{});

    std::cout << Arrow::TypeName<SMap1::get<Arrow::IntValueType<7>>>::Name() << std::endl;
    //  Arrow::print(SMap1::get<Arrow::IntValueType<7>>{});
     std::cout << typeid(SMap1::Pair).name() << std::endl;;
}
