#pragma once

#include <log4cplus/log4cplus.h>

namespace Arrow
{

namespace Log
{

class LogImplNull
{
protected:
    LogImplNull() = default;
    LogImplNull(LogImplNull& t) = delete;
    LogImplNull& operator=(const LogImplNull& t) = delete;
    virtual ~LogImplNull() = default;

public:
    template<typename ...Args>
    static bool init(Args... args)
    {
        return true;
    }

    // template<Em_Log_Level loglevel, typename TFileName, typename TFunName, int line>
    // static void Log(const char* szInfo)
    // {
    //     LogImpl::template Log<loglevel, TFileName, TFunName, line>(szInfo);
    // }

    template <Em_Log_Level loglevel, typename TFileName, typename TFunName, int line, typename T>
    static void Log(const T& t)
    {
    }

    template <Em_Log_Level loglevel, typename TFileName, typename TFunName, int line, typename... Args>
    static void Log(const char* szFmt, Args... args)
    {
    }

    template <typename... Args>
    static void print(const char* fmt, Args... args)
    {
    }
};

} // namespace Log
} // namespace Arrow