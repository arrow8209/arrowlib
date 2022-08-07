#pragma once

namespace Arrow
{

namespace Log
{
template<typename TLogImpl>
class LogInterface
{
protected:
    LogInterface() = default;
    LogInterface(LogInterface& t) = delete;
    LogInterface& operator=(const LogInterface& t) = delete;
    virtual ~LogInterface() = default;

    typedef TLogImpl LogImpl;
public:
    template<typename ...Args>
    static bool log_init(Args... args)
    {
        return LogImpl::log_init(args);
    }

    template<typename TFileName, typename TFunName, int line>
    static void Trace_Log(const char* szInfo)
    {
        LogImpl::template Trace_Log<TFileName, TFunName, line>(szInfo);
    }

    template <typename T>
    static void Trace_Log(const T& t)
    {
        LogImpl::Trace_Log(t);
    }

    template <typename... Args>
    static void Trace_Log(const char* szFmt, Args... args)
    {
        LogImpl::Trace_Log(szFmt, args...);
    }

    static void Debug_Log(const char* szInfo)
    {
        LogImpl::Debug_Log(szInfo);
    }

    template <typename T>
    static void Debug_Log(const T& t)
    {
        LogImpl::Debug_Log(t);
    }

    template <typename... Args>
    static void Debug_Log(const char* szFmt, Args... args)
    {
        LogImpl::Debug_Log(szFmt, args...);
    }

    static void Info_Log(const char* szInfo)
    {
        LogImpl::Info_Log(szInfo);
    }

    template <typename T>
    static void Info_Log(const T& t)
    {
        LogImpl::Info_Log(t);
    }

    template <typename... Args>
    static void Info_Log(const char* szFmt, Args... args)
    {
        LogImpl::Info_Log(szFmt, args...);
    }

    static void Warn_Log(const char* szInfo)
    {
        LogImpl::Warn_Log(szInfo);
    }

    template <typename T>
    static void Warn_Log(const T& t)
    {
        LogImpl::Warn_Log(t);
    }

    template <typename... Args>
    static void Warn_Log(const char* szFmt, Args... args)
    {
        LogImpl::Warn_Log(szFmt, args...);
    }

    static void Error_Log(const char* szInfo)
    {
        LogImpl::Error_Log(szInfo);
    }

    template <typename T>
    static void Error_Log(const T& t)
    {
        LogImpl::Error_Log(t);
    }

    template <typename... Args>
    static void Error_Log(const char* szFmt, Args... args)
    {
        LogImpl::Error_Log(szFmt, args...);
    }

    static void Fatal_Log(const char* szInfo)
    {
        LogImpl::Fatal_Log(szInfo);
    }

    template <typename T>
    static void Fatal_Log(const T& t)
    {
        LogImpl::Fatal_Log(t);
    }

    template <typename... Args>
    static void Fatal_Log(const char* szFmt, Args... args)
    {
        LogImpl::Fatal_Log(szFmt, args...);
    }

    template <typename... Args>
    static void print(const char* fmt, Args... args)
    {
        Debug_Log(fmt, args...);
    }
};

} // namespace Log
} // namespace Arrow