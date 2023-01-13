/*
 * @FilePath: /arrowlib/arrow/other/std_assist.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-07-08 09:54:32
 * @Description: 针对std 的一些经常用到的操作的辅助函数
 */
#pragma once

#include "../log/log_interface.h"

#include <unistd.h>
#include <stdio.h>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include <cstring>

#ifdef _WIN32
#elif __APPLE__
#elif __linux__
#include <malloc.h>
#endif

// #ifdef _WIN32
//    //define something for Windows (32-bit and 64-bit, this part is common)
//    #ifdef _WIN64
//       //define something for Windows (64-bit only)
//    #else
//       //define something for Windows (32-bit only)
//    #endif
// #elif __APPLE__
//     #include "TargetConditionals.h"
//     #if TARGET_IPHONE_SIMULATOR
//          // iOS Simulator
//     #elif TARGET_OS_IPHONE
//         // iOS device
//     #elif TARGET_OS_MAC
//         // Other kinds of Mac OS
//     #else
//     #   error "Unknown Apple platform"
//     #endif
// #elif __ANDROID__
//     // android
// #elif __linux__
//     // linux
// #elif __unix__ // all unices not caught above
//     // Unix
// #elif defined(_POSIX_VERSION)
//     // POSIX
// #else
// #   error "Unknown compiler"
// #endif


namespace Arrow
{

namespace Other
{

static inline std::string get_app_path()
{


#ifdef WIN32
    char chPath[MAX_PATH];
    std::string strPath;
    int nCount;

    ::GetModuleFileName(NULL, chPath, MAX_PATH); //得到执行文件名的全路径
    strPath = chPath;
    nCount = strPath.find_last_of('\\');
    strPath = strPath.substr(0, nCount + 1);
    return strPath;

#elif __linux__
    std::string sPath;
    char buf[4097] = {0};
    long size;
    char* ptr;
    size = pathconf(".", _PC_PATH_MAX);
    if ((ptr = (char*)malloc((size_t)size)) != NULL)
    {
        memset(ptr, 0, size);
        sprintf(ptr, "/proc/%d/exe", getpid());
    }
    else
        return sPath;
    readlink(ptr, buf, size);
    free(ptr);
    ptr = NULL;
    sPath = buf;
    int nPos = sPath.find_last_of("/");
    return sPath.substr(0, nPos + 1);
#elif __APPLE__
    const int MAXPATH = 250;
    char buffer[MAXPATH];
    getcwd(buffer, MAXPATH);
    return std::string(buffer);
#endif
}


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
// template <Arrow::EmLogLevel _logLevel>
// static void PrintfMallinfo()
// {
//     struct mallinfo info = mallinfo();
//     ARROW_LOG(_logLevel, "===========================\n");
//     ARROW_LOG_FMT(_logLevel, "\tmallinfo\n");
//     ARROW_LOG_FMT(_logLevel, "\t\tarena:%d\n", info.arena);
//     ARROW_LOG_FMT(_logLevel, "\t\tordblks:%d\n", info.ordblks);
//     ARROW_LOG_FMT(_logLevel, "\t\tsmblks:%d\n", info.smblks);
//     ARROW_LOG_FMT(_logLevel, "\t\thblks:%d\n", info.hblks);
//     ARROW_LOG_FMT(_logLevel, "\t\thblkhd:%d\n", info.hblkhd);
//     ARROW_LOG_FMT(_logLevel, "\t\tusmblks:%d\n", info.usmblks);
//     ARROW_LOG_FMT(_logLevel, "\t\tuordblks:%d\n", info.uordblks);
//     ARROW_LOG_FMT(_logLevel, "\t\tfordblks:%d\n", info.fordblks);
//     ARROW_LOG_FMT(_logLevel, "\t\tkeepcost:%d\n", info.keepcost);
//     ARROW_LOG(_logLevel, "===========================\n");
// }
// static unsigned int StrToUInt32(const char* sz)
// {
//     const signed char* pCh;
//     unsigned int unRet = 5381;
//     for (pCh = (const signed char*)sz; *pCh != '\0'; pCh++)
//         unRet = (unRet << 5) + unRet + *pCh;
//     return unRet;
// }

// djb2 hash 算法 [zhuyb 2023-01-06 09:15:45]
static uint32_t StrToUInt32Impl(const char* sz, uint32_t u32Ret = 5381)
{
    const uint8_t* pCh;
    for (pCh = reinterpret_cast<const uint8_t*>(sz); *pCh != '\0'; pCh++)
        u32Ret = (u32Ret << 5) + u32Ret + *pCh;
    return u32Ret;
}


static uint32_t StrToUInt32(const char* sz)
{
    return StrToUInt32Impl(sz, 5381);
    // const uint8_t* pCh;
    // uint32_t unRet = 5381;
    // for (pCh = reinterpret_cast<const uint8_t*>(sz); *pCh != '\0'; pCh++)
    //     unRet = (unRet << 5) + unRet + *pCh;
    // return unRet;
}

// Args 必须为const char* 类型 [zhuyb 2023-01-06 10:34:31]
template<typename ...Args>
static uint32_t StrToUInt32(const char* sz, Args... args)
{
    uint32_t u32Ret = StrToUInt32(args...);
    return StrToUInt32Impl(sz, u32Ret);
}

inline static std::chrono::milliseconds get_milliseconds()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch());
}

} // namespace Std
}