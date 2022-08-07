/*
 * @FilePath: /arrowlib/log/log.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-07-07 18:26:24
 * @Description: 日志输出,使用log4cplus输出日志
 *
 *
 * SET(ENV{PKG_CONFIG_PATH} /home/zhuyb/Documents/2.lib/maxvision/lib/log4cplus/lib/pkgconfig:$ENV{PKG_CONFIG_PATH})
 * PKG_SEARCH_MODULE(log4cplus REQUIRED log4cplus)
 *
 */
#pragma once
#include "log_interface.h"
#include "log_impl_null.h"
// #include "log_impl_log4cplus.h"

#include <cxxabi.h> // abi need
#include <typeinfo> // typeid need

// get class name 尽量不要使用，这是运行期函数，比较消耗资源，建议使用 filename+行号
#define __CLASS__ __class__
#define __class__ abi::__cxa_demangle(typeid(*this).name(), 0, 0, &Arrow::Log::demangle_status)

namespace Arrow
{

namespace Log
{

//注意这里必须是static，否则会运行时报错多重定义
static int demangle_status; 

// typedef LogInterface<LogImplLog4Cplus> ALog;
// typedef LogInterface<LogImplNull> ALog;

// 这函数属于编译器期执行 [zhuyb 2022-07-20 11:10:12]
// template<size_t N>
// constexpr const char *get_file_basename(const char (&filename)[N])
// {
//     for (int i = N - 1; i >= 0; i--)
//     {
//         if (filename[i] == '/' || filename[i] == '\\')
//             return filename + i + 1;
//     }
//     return filename;
// }

} // namespace Log
} // namespace Arrow