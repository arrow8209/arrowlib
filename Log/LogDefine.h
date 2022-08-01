/*
 * @FilePath: /arrowlib/Log/LogDefine.h
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
#include "log_impl_log4cplus.h"

#include <cxxabi.h> // abi need
#include <typeinfo> // typeid need

// get class name 尽量不要使用，这是运行期函数，比较消耗资源，建议使用 filename+行号
//注意这里必须是static，否则会运行时报错多重定义
static int demangle_status; 
#define __CLASS__ __class__
#define __class__ abi::__cxa_demangle(typeid(*this).name(), 0, 0, &demangle_status)

#define INFO_LOG(fmt, ...) fprintf(stdout, "[INFO]  " fmt "\n", ##__VA_ARGS__)
#define WARN_LOG(fmt, ...) fprintf(stdout, "[WARN]  " fmt "\n", ##__VA_ARGS__)
#define ERROR_LOG(fmt, ...) fprintf(stdout, "[ERROR] " fmt "\n", ##__VA_ARGS__)

#define g_logger ""

#define ARROW_LOG_TRACE(logEvent) LOG4CPLUS_TRACE(g_logger, logEvent)
#define ARROW_LOG_TRACE_FMT(...) LOG4CPLUS_TRACE_FMT(g_logger, __VA_ARGS__)

#define ARROW_LOG_DEBUG(logEvent) LOG4CPLUS_DEBUG(g_logger, logEvent)
#define ARROW_LOG_DEBUG_FMT(...) LOG4CPLUS_DEBUG_FMT(g_logger, __VA_ARGS__)

#define ARROW_LOG_INFO(logEvent) LOG4CPLUS_INFO(g_logger, logEvent)
#define ARROW_LOG_INFO_FMT(...) LOG4CPLUS_INFO_FMT(g_logger, __VA_ARGS__)

#define ARROW_LOG_WARN(logEvent) LOG4CPLUS_WARN(g_logger, logEvent)
#define ARROW_LOG_WARN_FMT(...) LOG4CPLUS_WARN_FMT(g_logger, __VA_ARGS__)

#define ARROW_LOG_ERROR(logEvent) LOG4CPLUS_ERROR(g_logger, logEvent)
#define ARROW_LOG_ERROR_FMT(...) LOG4CPLUS_ERROR_FMT(g_logger, __VA_ARGS__)

#define ARROW_LOG_FATAL(logEvent) LOG4CPLUS_FATAL(g_logger, logEvent)
#define ARROW_LOG_FATAL_FMT(...) LOG4CPLUS_FATAL_FMT(g_logger, __VA_ARGS__)


// #define ARROW_LOG_TRACE(logEvent) 
// #define ARROW_LOG_TRACE_FMT(...)

// #define ARROW_LOG_DEBUG(logEvent) 
// #define ARROW_LOG_DEBUG_FMT(...) 

// #define ARROW_LOG_INFO(logEvent) 
// #define ARROW_LOG_INFO_FMT(...) 

// #define ARROW_LOG_WARN(logEvent)
// #define ARROW_LOG_WARN_FMT(...) 

// #define ARROW_LOG_ERROR(logEvent) 
// #define ARROW_LOG_ERROR_FMT(...) 

// #define ARROW_LOG_FATAL(logEvent) 
// #define ARROW_LOG_FATAL_FMT(...)


namespace Arrow
{

namespace Log
{

typedef LogInterface<LogImplLog4Cplus> ALog;
// typedef LogInterface<LogImplNull> ALog;
// 这函数属于编译器期执行 [zhuyb 2022-07-20 11:10:12]
template<size_t N>
constexpr const char *get_file_basename(const char (&filename)[N])
{
    for (int i = N - 1; i >= 0; i--)
    {
        if (filename[i] == '/' || filename[i] == '\\')
            return filename + i + 1;
    }
    return filename;
}

} // namespace Log
} // namespace Arrow