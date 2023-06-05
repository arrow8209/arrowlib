#pragma once
#include "../typelist/typelist.h"

#define LOG_PARAM(LogLevel) LogLevel, STATIC_FILE, typename STATIC_FUNC, __LINE__
#define LOG_PARAM_FMT(LogLevel, fmt) LogLevel, STATIC_FILE, typename STATIC_FUNC, __LINE__, typename STATIC_STRING(fmt)

namespace Arrow
{

typedef enum _emLogLevel
{
    LogNull,
    LogTrace,
    LogDebug,
    LogInfo,
    LogWarn,
    LogError,
    LogFatal,
    LogMax
} EmLogLevel;

namespace Log
{
template<typename TLogImpl, EmLogLevel printLevel = LogTrace>
class base_log
{
protected:
    base_log() = default;
    base_log(base_log& t) = delete;
    base_log& operator=(const base_log& t) = delete;
    virtual ~base_log() = default;

    typedef TLogImpl LogImpl;
public:
    template<typename ...Args>
    static bool init(Args... args)
    {
        return LogImpl::init(args...);
    }

    // template<EmLogLevel loglevel, typename TFileName, typename TFunName, int line>
    // static void Log(const char* szInfo)
    // {
    //     LogImpl::template Log<loglevel, TFileName, TFunName, line>(szInfo);
    // }

    template <EmLogLevel loglevel, typename TFileName, typename TFunName, int line, typename T>
    static void Log(const T& t)
    {
        LogImpl::template Log<loglevel, TFileName, TFunName, line, T>(t);
    }

    template <EmLogLevel loglevel, typename TFileName, typename TFunName, int line>
    static void Log(const char* t)
    {
        LogImpl::template Log<loglevel, TFileName, TFunName, line, const char*>(t);
    }

    template <EmLogLevel loglevel, typename TFileName, typename TFunName, int line>
    static void Log(char* t)
    {
        LogImpl::template Log<loglevel, TFileName, TFunName, line, char*>(t);
    }

    template <EmLogLevel loglevel, typename TFileName, typename TFunName, int line, typename TFmt, typename... Args>
    static void Log(Args... args)
    {
        LogImpl::template Log<loglevel, TFileName, TFunName, line, TFmt, Args...>(args...);
    }

    template <typename... Args>
    static void print(const char* fmt, Args... args)
    {
        Log<LOG_PARAM(printLevel)>(fmt, args...);
    }
};

} // namespace Log
} // namespace Arrow