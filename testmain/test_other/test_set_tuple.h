#pragma once
#include <chrono>
#include "arrow/arrow.h"
#include "arrow/other/set_tuple.h"

static void TestSetTupe()
{
    std::tuple<int,float, double> t1(1,(float)2.0, (double)3.0);

    Arrow::Other::SetTuple(t1, 2,(float)3.0, (double)4.0);

    int n1 = 0;
    float n2 = 0.0;
    double n3 = 0.0;
    std::tie(n1, n2, n3) = t1;
    std::cout << n1 << " " << n2 << " " << n3 << std::endl;

    Arrow::Other::SetTuple(t1, n1, n2, n3);

    auto tp1 = std::chrono::steady_clock::now();
    for(size_t i = 0; i < 10000000; ++i)
    {
        Arrow::Other::SetTuple(t1, n1, n2, n3);
    }
    auto tp2 = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(tp2 - tp1).count() << std::endl;
    
    tp1 = std::chrono::steady_clock::now();
    for(size_t i = 0; i < 10000000; ++i)
    {
        t1 = std::make_tuple(1,(float)2.0, (double)3.0);
    }
    tp2 = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::microseconds>(tp2 - tp1).count() << std::endl;
}