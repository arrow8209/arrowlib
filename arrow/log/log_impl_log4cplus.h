#pragma once

#include <log4cplus/log4cplus.h>
#include <log4cplus/logger.h>

namespace Arrow
{

namespace Log
{



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

class LogImplLog4Cplus
{
protected:
    LogImplLog4Cplus() = default;
    LogImplLog4Cplus(LogImplLog4Cplus& t) = delete;
    LogImplLog4Cplus& operator=(const LogImplLog4Cplus& t) = delete;
    virtual ~LogImplLog4Cplus() = default;

public:
    static bool log_init(const char* szConfigFileName)
    {
        log4cplus::initialize();
        try
        {
            if (szConfigFileName != nullptr)
            {
                log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(szConfigFileName));
                return true;
            }
        }
        catch (...)
        {
            printf("log_init exception...\n");
        }

        log4cplus::Logger&& _logger = log4cplus::Logger::getRoot();

        if (_logger.getAllAppenders().size() == 0)
        {
            log4cplus::SharedAppenderPtr _append(new log4cplus::ConsoleAppender());
            _append->setName("root");
            std::string pattern = "[D]<%-5p>[%D{%Y-%m-%d %H:%M:%S}][%b,%L][%t]:%m%n";

            std::unique_ptr<log4cplus::Layout> _layout(new log4cplus::PatternLayout(pattern));
            _append->setLayout(std::move(_layout));
            _logger.setLogLevel(log4cplus::ALL_LOG_LEVEL);
            _logger.addAppender(_append);
        }
        return true;
    }

    template <typename TFileName, typename TFunName, int line>
    static void Trace_Log(const char* szInfo)
    {
        // LOG4CPLUS_SUPPRESS_DOWHILE_WARNING()
        // do
        // {
        //     log4cplus::Logger const& _l = log4cplus::detail::macros_get_logger(g_logger);
        //     if (LOG4CPLUS_MACRO_LOGLEVEL_PRED(_l.isEnabledFor(log4cplus::TRACE_LOG_LEVEL), logLevel))
        //     {
        //         log4cplus::detail::macro_forced_log(_l,
        //                                             log4cplus::TRACE_LOG_LEVEL, logEvent,
        //                                             LOG4CPLUS_MACRO_FILE(),
        //                                             __LINE__,
        //                                             LOG4CPLUS_MACRO_FUNCTION());
        //     }
        // } while (0)
        // LOG4CPLUS_RESTORE_DOWHILE_WARNING()

        typedef Arrow::typelist::tvaluelist_to_data<TFileName> Staic_FileName;
        typedef Arrow::typelist::tvaluelist_to_data<TFunName> Staic_FunName;

        LOG4CPLUS_SUPPRESS_DOWHILE_WARNING()
        do
        {
            log4cplus::Logger const& _l = log4cplus::detail::macros_get_logger(g_logger);
            if (_l.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
            {
                LOG4CPLUS_MACRO_INSTANTIATE_OSTRINGSTREAM(_log4cplus_buf);
                _log4cplus_buf << szInfo;
                log4cplus::detail::macro_forced_log(_l,
                                                    log4cplus::TRACE_LOG_LEVEL, _log4cplus_buf.str(),
                                                    Staic_FileName::data, 
                                                    line,
                                                    Staic_FunName::data);
            }
        } while (0);
        LOG4CPLUS_RESTORE_DOWHILE_WARNING()
    }

    template <typename T>
    static void Trace_Log(const T& t)
    {
        LOG4CPLUS_TRACE("", t);
    }

    template <typename TFileName, typename TFunName, int line, typename... Args>
    static void Trace_Log(const char* szFmt, Args... args)
    {
        LOG4CPLUS_SUPPRESS_DOWHILE_WARNING()
        do
        {
            log4cplus::Logger const& _l = log4cplus::detail::macros_get_logger("");
            if (LOG4CPLUS_MACRO_LOGLEVEL_PRED(
                    _l.isEnabledFor(log4cplus::TRACE_LOG_LEVEL), TRACE_LOG_LEVEL))
            {
                // LOG4CPLUS_MACRO_INSTANTIATE_SNPRINTF_BUF(_snpbuf);
                log4cplus::helpers::snprintf_buf& _snpbuf = log4cplus::detail::get_macro_body_snprintf_buf();

                log4cplus::tchar const* _logEvent = _snpbuf.print(szFmt, args...);
                log4cplus::detail::macro_forced_log(_l,
                                                    log4cplus::TRACE_LOG_LEVEL, _logEvent,
                                                    LOG4CPLUS_MACRO_FILE(), __LINE__,
                                                    LOG4CPLUS_MACRO_FUNCTION());
            }
        } while (0)
        LOG4CPLUS_RESTORE_DOWHILE_WARNING();
    }

    static void Debug_Log(const char* szInfo)
    {
        LOG4CPLUS_DEBUG_STR("", szInfo);
    }

    template <typename T>
    static void Debug_Log(const T& t)
    {
        LOG4CPLUS_DEBUG("", t);
    }

    template <typename... Args>
    static void Debug_Log(const char* szFmt, Args... args)
    {
        LOG4CPLUS_DEBUG_FMT("", szFmt, args...);
    }

    static void Info_Log(const char* szInfo)
    {
        LOG4CPLUS_INFO_STR("", szInfo);
    }

    template <typename T>
    static void Info_Log(const T& t)
    {
        LOG4CPLUS_INFO("", t);
    }

    template <typename... Args>
    static void Info_Log(const char* szFmt, Args... args)
    {
        LOG4CPLUS_INFO_FMT("", szFmt, args...);
    }

    static void Warn_Log(const char* szInfo)
    {
        LOG4CPLUS_WARN_STR("", szInfo);
    }

    template <typename T>
    static void Warn_Log(const T& t)
    {
        LOG4CPLUS_WARN("", t);
    }

    template <typename... Args>
    static void Warn_Log(const char* szFmt, Args... args)
    {
        LOG4CPLUS_WARN_FMT("", szFmt, args...);
    }

    static void Error_Log(const char* szInfo)
    {
        LOG4CPLUS_ERROR_STR("", szInfo);
    }

    template <typename T>
    static void Error_Log(const T& t)
    {
        LOG4CPLUS_ERROR("", t);
    }

    template <typename... Args>
    static void Error_Log(const char* szFmt, Args... args)
    {
        LOG4CPLUS_ERROR_FMT("", szFmt, args...);
    }

    static void Fatal_Log(const char* szInfo)
    {
        LOG4CPLUS_FATAL_STR("", szInfo);
    }

    template <typename T>
    static void Fatal_Log(const T& t)
    {
        LOG4CPLUS_FATAL("", t);
    }

    template <typename... Args>
    static void Fatal_Log(const char* szFmt, Args... args)
    {
        LOG4CPLUS_FATAL_FMT("", szFmt, args...);
    }

    template <typename... Args>
    static void print(const char* fmt, Args... args)
    {
        Debug_Log(fmt, args...);
    }
};

} // namespace Log
} // namespace Arrow