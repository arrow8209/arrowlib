/*
 * @FilePath: /ai_server/ShareCode/arrow/log.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-12-14 15:13:25
 * @Description: 日志具体使用类，根据实际情况重写这个文件来实现特定的日志
 */
#pragma once
#include "./log/log_impl_log4cplus.h"
#include "./log/log_impl_null.h"
#include "./log/log_impl_std.h"
#include "./log/log_interface.h"

// 在不同的位置定义这句话实现不同的日志输出 [zhuyb 2022-12-15 09:55:55]
typedef Arrow::Log::base_log<Arrow::Log::LogImplLog4Cplus> ArrowLog;

// #define ARROW_LOG(logLevel, logEvent) ArrowLog::Log<logLevel, STATIC_FILE, typename STATIC_FUNC, __LINE__>(logEvent);
// #define ARROW_LOG_FMT(logLevel, ...) ArrowLog::Log<logLevel, STATIC_FILE, typename STATIC_FUNC, __LINE__>(__VA_ARGS__);

// #define ARROW_LOG_TRACE(logEvent) ArrowLog::Log<LOG_PARAM(Arrow::LogTrace), const char*>(logEvent);
// #define ARROW_LOG_TRACE_FMT(fmt, ...) ArrowLog::Log<LOG_PARAM_FMT(Arrow::LogTrace, fmt)>(__VA_ARGS__);

// #define ARROW_LOG_DEBUG(logEvent) ArrowLog::Log<LOG_PARAM(Arrow::LogDebug), const char*>(logEvent);
// #define ARROW_LOG_DEBUG_FMT(fmt, ...) ArrowLog::Log<LOG_PARAM_FMT(Arrow::LogDebug, fmt)>(__VA_ARGS__);

// #define ARROW_LOG_INFO(logEvent) ArrowLog::Log<LOG_PARAM(Arrow::LogInfo), const char*>(logEvent);
// #define ARROW_LOG_INFO_FMT(fmt, ...) ArrowLog::Log<LOG_PARAM_FMT(Arrow::LogInfo, fmt)>(__VA_ARGS__);

// #define ARROW_LOG_WARN(logEvent) ArrowLog::Log<LOG_PARAM(Arrow::LogWarn), const char*>(logEvent);
// #define ARROW_LOG_WARN_FMT(fmt, ...) ArrowLog::Log<LOG_PARAM_FMT(Arrow::LogWarn, fmt)>(__VA_ARGS__);

// #define ARROW_LOG_ERROR(logEvent) ArrowLog::Log<LOG_PARAM(Arrow::LogError), const char*>(logEvent);
// #define ARROW_LOG_ERROR_FMT(fmt, ...) ArrowLog::Log<LOG_PARAM_FMT(Arrow::LogError, fmt)>(__VA_ARGS__);

// #define ARROW_LOG_FATAL(logEvent) ArrowLog::Log<LOG_PARAM(Arrow::LogFatal), const char*>(logEvent);
// #define ARROW_LOG_FATAL_FMT(fmt, ...) ArrowLog::Log<LOG_PARAM_FMT(Arrow::LogFatal, fmt)>(__VA_ARGS__);

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