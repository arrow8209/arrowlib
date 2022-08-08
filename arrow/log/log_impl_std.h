#pragma once
#include "../typelist/typelist.h"

namespace Arrow
{

namespace Log
{

namespace details
{

}


class LogImplDefault
{
protected:
    LogImplNull() = default;
    LogImplNull(LogImplNull& t) = delete;
    LogImplNull& operator=(const LogImplNull& t) = delete;
    virtual ~LogImplNull() = default;

public:
    static bool log_init(const char* szConfigFileName)
    {
        return true;
    }

    template <typename TFileName, typename TFunName, int line>
    static void Trace_Log(const char* szInfo)
    {
        std::cout<<""
    }

    template <typename TFileName, typename TFunName, int line, typename T>
    static void Trace_Log(const T& t)
    {
    }

    template <typename TFileName, typename TFunName, int line, typename... Args>
    static void Trace_Log(const char* szFmt, Args... args)
    {
    }

    static void Debug_Log(const char* szInfo)
    {
      
    }

    template <typename T>
    static void Debug_Log(const T& t)
    {
    }

    template <typename... Args>
    static void Debug_Log(const char* szFmt, Args... args)
    {
    }

    static void Info_Log(const char* szInfo)
    {
       
    }

    template <typename T>
    static void Info_Log(const T& t)
    {
       
    }

    template <typename... Args>
    static void Info_Log(const char* szFmt, Args... args)
    {
      
    }

    static void Warn_Log(const char* szInfo)
    {
    }

    template <typename T>
    static void Warn_Log(const T& t)
    {
    }

    template <typename... Args>
    static void Warn_Log(const char* szFmt, Args... args)
    {
    }

    static void Error_Log(const char* szInfo)
    {
    }

    template <typename T>
    static void Error_Log(const T& t)
    {
    }

    template <typename... Args>
    static void Error_Log(const char* szFmt, Args... args)
    {
    }

    static void Fatal_Log(const char* szInfo)
    {
    }

    template <typename T>
    static void Fatal_Log(const T& t)
    {
    }

    template <typename... Args>
    static void Fatal_Log(const char* szFmt, Args... args)
    {
    }

    template <typename... Args>
    static void print(const char* fmt, Args... args)
    {
    }
};

} // namespace Log
} // namespace Arrow