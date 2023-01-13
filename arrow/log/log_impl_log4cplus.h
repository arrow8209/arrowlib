#pragma once

#include <log4cplus/log4cplus.h>
#include <log4cplus/logger.h>
#include <string.h>
#include "log_interface.h"
#include "../typelist/typelist.h"

namespace Arrow
{

namespace Log
{
namespace details_log4
{
    typedef  Arrow::static_map<
     Arrow::static_pair<Arrow::value_type<LogNull>,Arrow::value_type<log4cplus::TRACE_LOG_LEVEL>>,
     Arrow::static_pair<Arrow::value_type<LogTrace>,Arrow::value_type<log4cplus::TRACE_LOG_LEVEL>>,
     Arrow::static_pair<Arrow::value_type<LogDebug>,Arrow::value_type<log4cplus::DEBUG_LOG_LEVEL>>,
     Arrow::static_pair<Arrow::value_type<LogInfo>,Arrow::value_type<log4cplus::INFO_LOG_LEVEL>>,
     Arrow::static_pair<Arrow::value_type<LogWarn>,Arrow::value_type<log4cplus::WARN_LOG_LEVEL>>,
     Arrow::static_pair<Arrow::value_type<LogError>,Arrow::value_type<log4cplus::ERROR_LOG_LEVEL>>,
     Arrow::static_pair<Arrow::value_type<LogFatal>,Arrow::value_type<log4cplus::FATAL_LOG_LEVEL>>,
     Arrow::static_pair<Arrow::value_type<LogMax>,Arrow::value_type<log4cplus::TRACE_LOG_LEVEL>>
     > Log4Type;

    // 不使用CPU优化的日志类型 [zhuyb 2022-08-12 10:34:19]
    typedef Arrow::value_typelist<LogNull, LogTrace, LogDebug, LogInfo, LogMax> UnLinly;
}


// #define INFO_LOG(fmt, ...) fprintf(stdout, "[INFO]  " fmt "\n", ##__VA_ARGS__)
// #define WARN_LOG(fmt, ...) fprintf(stdout, "[WARN]  " fmt "\n", ##__VA_ARGS__)
// #define ERROR_LOG(fmt, ...) fprintf(stdout, "[ERROR] " fmt "\n", ##__VA_ARGS__)

// #define g_logger ""

// #define ARROW_LOG_TRACE(logEvent) LOG4CPLUS_TRACE(g_logger, logEvent)
// #define ARROW_LOG_TRACE_FMT(...) LOG4CPLUS_TRACE_FMT(g_logger, __VA_ARGS__)

// #define ARROW_LOG_DEBUG(logEvent) LOG4CPLUS_DEBUG(g_logger, logEvent)
// #define ARROW_LOG_DEBUG_FMT(...) LOG4CPLUS_DEBUG_FMT(g_logger, __VA_ARGS__)

// #define ARROW_LOG_INFO(logEvent) LOG4CPLUS_INFO(g_logger, logEvent)
// #define ARROW_LOG_INFO_FMT(...) LOG4CPLUS_INFO_FMT(g_logger, __VA_ARGS__)

// #define ARROW_LOG_WARN(logEvent) LOG4CPLUS_WARN(g_logger, logEvent)
// #define ARROW_LOG_WARN_FMT(...) LOG4CPLUS_WARN_FMT(g_logger, __VA_ARGS__)

// #define ARROW_LOG_ERROR(logEvent) LOG4CPLUS_ERROR(g_logger, logEvent)
// #define ARROW_LOG_ERROR_FMT(...) LOG4CPLUS_ERROR_FMT(g_logger, __VA_ARGS__)

// #define ARROW_LOG_FATAL(logEvent) LOG4CPLUS_FATAL(g_logger, logEvent)
// #define ARROW_LOG_FATAL_FMT(...) LOG4CPLUS_FATAL_FMT(g_logger, __VA_ARGS__)


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
    static bool init(const char* szConfigFileName)
    {
        // constexpr static char szConsole[]={"consle_default"};
        log4cplus::initialize();
        log4cplus::Logger&& logger = log4cplus::Logger::getRoot();  
        log4cplus::SharedAppenderPtrList pList = logger.getAllAppenders();

        if (pList.size() > 1)
        {
            log4cplus::SharedAppenderPtrList::iterator it = pList.begin();
            printf("log4cplus has been initialized appenders size:%u\n", static_cast<uint32_t>(pList.size()));
            for (; it != pList.end(); ++it)
            {
                printf("Appender name:%s\n", it->get()->getName().c_str());
            }
            return true;
        }

        // if (pList.size() == 1 &&
        //     strcmp(szConsole, pList.begin()->get()->getName().c_str()) != 0)
        // {
        //     printf("Appender name:%s\n", pList.begin()->get()->getName().c_str());
        //     return true;
        // }

        try
        {
            if (szConfigFileName != nullptr)
            {
                log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(szConfigFileName));
            }
        }
        catch(const std::exception& e)
        {
            std::cout << "doConfigure:" << e.what() << '\n';
        }

        try
        {
            if (logger.getAllAppenders().size() == 0)
            {
                // 设置TRACE~INFO信息日志格式 [zhuyb 2022-11-16 08:54:33]
                log4cplus::SharedAppenderPtr appendTraceToInfo(new log4cplus::ConsoleAppender());
                appendTraceToInfo->setName("traceConsole");
                std::string patternTraceToInfo = "[D]<%-5p>[%D{%Y-%m-%d %H:%M:%S}][%b,%L][%t]:%m%n";

                std::unique_ptr<log4cplus::Layout> layoutTraceToInfo(new log4cplus::PatternLayout(patternTraceToInfo));
                appendTraceToInfo->setLayout(std::move(layoutTraceToInfo));

                log4cplus::helpers::Properties pro;
                pro.setProperty("LogLevelMin", "TRACE");
                pro.setProperty("LogLevelMax", "INFO");
                
                log4cplus::spi::FilterPtr filterTraceToInfo(new log4cplus::spi::LogLevelRangeFilter(pro));
                appendTraceToInfo->addFilter(filterTraceToInfo);
                // logger.setLogLevel(log4cplus::ALL_LOG_LEVEL);
                logger.addAppender(appendTraceToInfo);



                // 设置WARN~FATAL信息日志格式 [zhuyb 2022-11-16 08:54:45]
                log4cplus::SharedAppenderPtr appendWarnToFatal(new log4cplus::ConsoleAppender());
                appendWarnToFatal->setName("warnConsole");
                std::string patternWarnToFatal = "\033[1;31m[D]<%-5p>[%D{%Y-%m-%d %H:%M:%S}][%b,%L][%t]:%m%n\033[0m";
                // std::string patternWarnToFatal = "\033[1;31m[D]<%-5p>%n\033[0m";

                std::unique_ptr<log4cplus::Layout> layoutWarnToFatal(new log4cplus::PatternLayout(patternWarnToFatal));
                appendWarnToFatal->setLayout(std::move(layoutWarnToFatal));

                pro.setProperty("LogLevelMin", "WARN");
                pro.setProperty("LogLevelMax", "FATAL");
                
                log4cplus::spi::FilterPtr filterWarnToFatal(new log4cplus::spi::LogLevelRangeFilter(pro));
                appendWarnToFatal->addFilter(filterWarnToFatal);

                logger.addAppender(appendWarnToFatal);
            }
        }
        catch(const std::exception& e)
        {
            std::cout << "set console fail error msg :" << e.what() << '\n';
        }
        
        
        return true;
    }

    // template <typename TFileName, typename TFunName, int line>
    // static void Trace_Log(const char* szInfo)
    // {
    //     // LOG4CPLUS_SUPPRESS_DOWHILE_WARNING()
    //     // do
    //     // {
    //     //     log4cplus::Logger const& _l = log4cplus::detail::macros_get_logger(g_logger);
    //     //     if (LOG4CPLUS_MACRO_LOGLEVEL_PRED(_l.isEnabledFor(log4cplus::TRACE_LOG_LEVEL), logLevel))
    //     //     {
    //     //         log4cplus::detail::macro_forced_log(_l,
    //     //                                             log4cplus::TRACE_LOG_LEVEL, logEvent,
    //     //                                             LOG4CPLUS_MACRO_FILE(),
    //     //                                             __LINE__,
    //     //                                             LOG4CPLUS_MACRO_FUNCTION());
    //     //     }
    //     // } while (0)
    //     // LOG4CPLUS_RESTORE_DOWHILE_WARNING()

    //     typedef Arrow::tlist::tvaluelist_to_data<TFileName> Staic_FileName;
    //     typedef Arrow::tlist::tvaluelist_to_data<TFunName> Staic_FunName;

    //     LOG4CPLUS_SUPPRESS_DOWHILE_WARNING()
    //     do
    //     {
    //         log4cplus::Logger const& _l = log4cplus::detail::macros_get_logger(g_logger);
    //         if (_l.isEnabledFor(log4cplus::TRACE_LOG_LEVEL))
    //         {
    //             LOG4CPLUS_MACRO_INSTANTIATE_OSTRINGSTREAM(_log4cplus_buf);
    //             _log4cplus_buf << szInfo;
    //             log4cplus::detail::macro_forced_log(_l,
    //                                                 log4cplus::TRACE_LOG_LEVEL, _log4cplus_buf.str(),
    //                                                 Staic_FileName::data, 
    //                                                 line,
    //                                                 Staic_FunName::data);
    //         }
    //     } while (0);
    //     LOG4CPLUS_RESTORE_DOWHILE_WARNING()
    // }

    // template <typename T>
    // static void Trace_Log(const T& t)
    // {
    //     LOG4CPLUS_TRACE("", t);
    // }

    // template <typename TFileName, typename TFunName, int line, typename... Args>
    // static void Trace_Log(const char* szFmt, Args... args)
    // {
    //     LOG4CPLUS_SUPPRESS_DOWHILE_WARNING()
    //     do
    //     {
    //         log4cplus::Logger const& _l = log4cplus::detail::macros_get_logger("");
    //         if (LOG4CPLUS_MACRO_LOGLEVEL_PRED(
    //                 _l.isEnabledFor(log4cplus::TRACE_LOG_LEVEL), TRACE_LOG_LEVEL))
    //         {
    //             // LOG4CPLUS_MACRO_INSTANTIATE_SNPRINTF_BUF(_snpbuf);
    //             log4cplus::helpers::snprintf_buf& _snpbuf = log4cplus::detail::get_macro_body_snprintf_buf();

    //             log4cplus::tchar const* _logEvent = _snpbuf.print(szFmt, args...);
    //             log4cplus::detail::macro_forced_log(_l,
    //                                                 log4cplus::TRACE_LOG_LEVEL, _logEvent,
    //                                                 LOG4CPLUS_MACRO_FILE(), __LINE__,
    //                                                 LOG4CPLUS_MACRO_FUNCTION());
    //         }
    //     } while (0)
    //     LOG4CPLUS_RESTORE_DOWHILE_WARNING();
    // }

    // typename std::enable_if<tlist::find<value_type<loglevel>, details::UnLinly>::value != -1>::type = void>
    template <EmLogLevel loglevel, typename TFileName, typename TFunName, int line, typename T>
    static void Log(const T& t)
    {
        do
        {   const int& log4cplus_loglevel = Arrow::smap::get<Arrow::value_type<loglevel>, details_log4::Log4Type>::Pair::value;
            log4cplus::Logger const& _l = log4cplus::detail::macros_get_logger("");
            
            if (LOG4CPLUS_UNLIKELY(_l.isEnabledFor(log4cplus_loglevel)))
            {
                LOG4CPLUS_MACRO_INSTANTIATE_OSTRINGSTREAM(_log4cplus_buf);
                _log4cplus_buf << t;
                log4cplus::detail::macro_forced_log(_l,
                                                    log4cplus_loglevel, _log4cplus_buf.str(),
                                                    tlist::tvaluelist_to_data<TFileName>::data, 
                                                    line,
                                                    tlist::tvaluelist_to_data<TFunName>::data);
            }
        } while (0);
    }

    template <EmLogLevel loglevel, typename TFileName, typename TFunName, int line, typename TFmt, typename... Args>
    static void Log(Args... args)
    {
        do
        {
            const int& log4cplus_loglevel = Arrow::smap::get<Arrow::value_type<loglevel>, details_log4::Log4Type>::Pair::value;
            log4cplus::Logger const& _l = log4cplus::detail::macros_get_logger("");
            if (LOG4CPLUS_UNLIKELY(_l.isEnabledFor(log4cplus_loglevel)))
            {
                LOG4CPLUS_MACRO_INSTANTIATE_SNPRINTF_BUF(_snpbuf);
                log4cplus::tchar const* _logEvent = _snpbuf.print(tlist::tvaluelist_to_data<TFmt>::data, args...);
                log4cplus::detail::macro_forced_log(_l,
                                                    log4cplus_loglevel,
                                                    _logEvent,
                                                    tlist::tvaluelist_to_data<TFileName>::data,
                                                    line,
                                                    tlist::tvaluelist_to_data<TFunName>::data);
            }
        } while (0);
    }

protected:

};

} // namespace Log
} // namespace Arrow