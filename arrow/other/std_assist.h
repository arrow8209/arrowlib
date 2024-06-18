/*
 * @FilePath: arrow/other/std_assist.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-07-08 09:54:32
 * @Description: 针对std 的一些经常用到的操作的辅助函数
 */
#pragma once

#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <chrono>
#include <iostream>
#include <sstream>
#include <thread>
#include <cstring>
#include <vector>
#include <memory>
#include <set>
#include <iostream>
#include <codecvt> // codecvt_utf8
#include <locale>  // wstring_convert
#include <limits>
#include <atomic>
#include <sstream>
#include <fstream>
#include <string>
#include <cctype>

#ifdef _WIN32
#elif __APPLE__
#elif __linux__
#include <malloc.h>
#include <pthread.h>
#include <dlfcn.h>
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

inline static std::string Trim(std::string& str)
{
    std::string blanks("\f\v\r\t\n ");
    str.erase(0, str.find_first_not_of(blanks));
    str.erase(str.find_last_not_of(blanks) + 1);
    return str;
}

// 去除重复，并将结果放入dst中 [zhuyb 2023-02-21 09:42:12]
template <typename T>
void RemoveRepeat(std::vector<T>& src, std::vector<T>& dst)
{
    // typedef std::vector<T> VecSharedPtr;
    // if(src.size() <= 1)
    // {
    //     dst = src;
    //     return;
    // }

    // typename VecSharedPtr::iterator it1 = src.begin();
    // typename VecSharedPtr::iterator it2 = src.begin();
    // typename VecSharedPtr::iterator itEnd = src.end();
    // it2++;
    // while(it2 != itEnd)
    // {
    //     if(std::find(it2, itEnd, *it1) == itEnd)
    //     {
    //         dst.push_back(*it1);
    //     }
    //     it1++;
    //     it2++;
    // }
    // dst.push_back(*it1);
    std::set<T> setVal(src.begin(), src.end());
    dst.insert(dst.end(), setVal.begin(), setVal.end());
}

// 去除重复 [zhuyb 2023-02-21 09:42:34]
template<typename T>
inline void RemoveRepeat(std::vector<T>& val)
{
    std::set<T> setVal(val.begin(), val.end());
    val.assign(setVal.begin(), setVal.end());
}

// /*
// * 宽字节转utf8
// */
// std::string wstring_2_utf8(std::wstring& wide_string) {
//     static std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
//     return utf8_conv.to_bytes(wide_string);
// }
// 
// /*
// * uft8转宽字节
// */
// std::wstring wstring_from_utf8(std::string& bype_string)
// {
//     static std::wstring_convert<std::codecvt_utf8<wchar_t>> utf8_conv;
//     return utf8_conv.from_bytes(bype_string);
// }

/*
* 多字节转宽字节
* 本函数会返回new分配的outStr，需要手动调用delete[]释放内存.
*/
static bool CA2W(wchar_t*& outStr, size_t& outLen, const char* inStr, int inLen)
{
    // size_t len = inLen + 1;
    // size_t converted = 0;
    // *outStr = new wchar_t[ len * sizeof(wchar_t)];
    // mbstowcs_s(&converted, *outStr, len, inStr, _TRUNCATE);
    // outLen = converted;
    // (*outStr)[len - 1] = L'\0';

    size_t len = 0;

    if (len < 1)
    {
        setlocale(LC_CTYPE,
                  "C.UTF-8"); // 字符集设置不当，mbstowcs返回值小于1(可通过异常返回值自动尝试并找出正确的字符集)。
        len = (int)mbstowcs(NULL, inStr, 0) + 1;
    }

    if (len < 1)
    {
        setlocale(LC_CTYPE,
                  "zh_CN.utf8"); // 字符集设置不当，mbstowcs返回值小于1(可通过异常返回值自动尝试并找出正确的字符集)。
        len = (int)mbstowcs(NULL, inStr, 0) + 1;
    }
    if (len < 1)
    {
        setlocale(LC_CTYPE,
                  "en_US.utf8"); // 字符集设置不当，mbstowcs返回值小于1(可通过异常返回值自动尝试并找出正确的字符集)。
        len = (int)mbstowcs(NULL, inStr, 0) + 1;
    }

    if(len < 1)
    {
        return false;
    }

    outStr = new wchar_t[len];
    swprintf(outStr, len, L"%hs", inStr);

    return true;
}

// /*
// * 宽字节转多字节
// * 本函数会返回new分配的outStr，需要手动调用delete[]释放内存.
// */
// void W2CA(char** outStr, size_t& outLen, const wchar_t* inStr, int inLen)
// {
//     size_t len = inLen + 1;
//     size_t converted = 0;
//     *outStr = new char[len * sizeof(char)];
//     wcstombs_s(&converted, *outStr, len, inStr, _TRUNCATE);
//     outLen = converted;
//     (*outStr)[len - 1] = '\0';
// }

static std::string GetDirectoryPath(const std::string& filePath)
{
    size_t pos = filePath.find_last_of("/\\");
    if (pos == std::string::npos)
    {
        return "";
    }
    return filePath.substr(0, pos + 1);
}

static void SetCurrentThreadName(const std::string& name) {
    
    #ifdef __APPLE__
        pthread_setname_np(name.c_str());
    #elif defined(__linux__)
        pthread_t thread = pthread_self();
        pthread_setname_np(thread,name.c_str());
    #else
        // windows [zhuyb 2023-06-05 22:01:53]
    #endif
}

static std::string GetFileName(const std::string& filePath)
{
    size_t pos = filePath.find_last_of("/\\");
    if (pos == std::string::npos)
    {
        return "";
    }
    return filePath.substr(pos + 1, filePath.size() - pos - 1);
}

#ifdef __linux__
static std::string GetSharedObjectFullName()
{
    Dl_info info;
    if (dladdr(reinterpret_cast<void*>(&GetSharedObjectFullName), &info))
    {
        return std::string(info.dli_fname);
    }
    return std::string("");
}
static std::string GetSharedObjectPath()
{
    std::string strTmp = GetSharedObjectFullName();
    if(strTmp.empty())
        return strTmp;
    return GetDirectoryPath(strTmp);
}

#endif

static bool IsBinaryFile(const std::string& filePath)
{
    std::ifstream file(filePath, std::ios::binary);
    if (!file)
    {
        std::cerr << "Unable to open file: " << filePath << std::endl;
        return false;
    }

    char ch;
    while (file.get(ch))
    {
        if (std::iscntrl(static_cast<unsigned char>(ch)) && !std::isspace(static_cast<unsigned char>(ch)))
        {
            // 检测到控制字符（如0-31，127），且不是空白字符（如 \t, \n, \r）
            file.close();
            return true; // 认为是二进制文件
        }
    }

    file.close();
    return false; // 没有检测到控制字符，认为是文本文件
}

} // namespace Std
}