#pragma once
#include <tuple>

template<typename ...Args>
class TestArgs
{
public:
    void Set(Args... args)
    {
        m_Args = std::make_tuple(args...);
    }

    void Get(Args&... args)
    {
        std::tie(args...) = m_Args;
    }

public:
    std::tuple<Args...> m_Args;
};

void MainTestArgs()
{
    TestArgs<int, float, double> testArgs;
    testArgs.Set((int)1, (float)2.0f, (double)3.0);
    int n1 = 0;
    float f2 = 0;
    double d3 = 0;
    testArgs.Get(n1, f2, d3);
}