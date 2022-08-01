/*
 * @FilePath: /plugin-based_streaming_data_analysis/ShareCode/Other/StdAssist.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-07-08 09:54:32
 * @Description: 针对std 的一些经常用到的操作的辅助函数
 */
#pragma once

#include <iostream>
#include <sstream>
#include <thread>
#include <malloc.h>
#include <chrono>
#include "Print.h"
namespace Arrow
{

namespace Other
{
class Assist
{
protected:
    Assist() = default;
    Assist(Assist& t) = delete;
    Assist& operator=(const Assist& t) = delete;
    virtual ~Assist() = default;

public:
    static uint64_t GetThreadID(const std::thread& th)
    {
        std::ostringstream oss;
        oss << th.get_id();
        return std::stoull(oss.str());
    }

    static uint64_t GetThreadID()
    {
        std::ostringstream oss;
        oss << std::this_thread::get_id();
        return std::stoull(oss.str());
    }

    template<typename TPrint = Arrow::Other::CPrint>
    static void PrintfMallinfo()
    {
        struct mallinfo info = mallinfo();
        TPrint::print("===========================\n");
        TPrint::print("\tmallinfo\n");
        TPrint::print("\t\tarena:%d\n", info.arena);
        TPrint::print("\t\tordblks:%d\n", info.ordblks);
        TPrint::print("\t\tsmblks:%d\n", info.smblks);
        TPrint::print("\t\thblks:%d\n", info.hblks);
        TPrint::print("\t\thblkhd:%d\n", info.hblkhd);
        TPrint::print("\t\tusmblks:%d\n", info.usmblks);
        TPrint::print("\t\tuordblks:%d\n", info.uordblks);
        TPrint::print("\t\tfordblks:%d\n", info.fordblks);
        TPrint::print("\t\tkeepcost:%d\n", info.keepcost);
        TPrint::print("===========================\n");
    }

    static unsigned int StrToUnit32(const char* sz)
    {
        const signed char* pCh;
        unsigned int unRet = 5381;

        for (pCh = sz; *pCh != '\0'; pCh++)
            unRet = (unRet << 5) + unRet + *pCh;

        return unRet;
    }

    template <size_t N, size_t M>
    constexpr const char* static_string_sub(const char (&sz)[N])
    {
        static_assert(M >= N, "需要截断的部分，超过字符串的长度");
        return sz + M;
    }

    inline static std::chrono::milliseconds get_milliseconds()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   std::chrono::system_clock::now().time_since_epoch());
    }
};

} // namespace Std
}