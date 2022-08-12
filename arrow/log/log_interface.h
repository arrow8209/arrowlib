#pragma once
#include "../typelist/typelist.h"

#define Log_Param(LogLevel) LogLevel, STATIC_FILE, STATIC_FUNC, __LINE__

namespace Arrow
{

typedef enum em_log_Level
{
    Log_Null,
    Log_Trace,
    Log_Debug,
    Log_Info,
    Log_Warn,
    Log_Error,
    Log_Fatal,
    Log_Max
} Em_Log_Level;

namespace Log
{
template<typename TLogImpl, Em_Log_Level printLevel = Log_Trace>
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

    // template<Em_Log_Level loglevel, typename TFileName, typename TFunName, int line>
    // static void Log(const char* szInfo)
    // {
    //     LogImpl::template Log<loglevel, TFileName, TFunName, line>(szInfo);
    // }

    template <Em_Log_Level loglevel, typename TFileName, typename TFunName, int line, typename T>
    static void Log(const T& t)
    {
        LogImpl::template Log<loglevel, TFileName, TFunName, line, T>(t);
    }

    template <Em_Log_Level loglevel, typename TFileName, typename TFunName, int line, typename... Args>
    static void Log(const char* szFmt, Args... args)
    {
        LogImpl::template Log<loglevel, TFileName, TFunName, line, Args...>(szFmt, args...);
    }

    template <typename... Args>
    static void print(const char* fmt, Args... args)
    {
        Log<Log_Param(printLevel)>(fmt, args...);
    }
};

} // namespace Log
} // namespace Arrow