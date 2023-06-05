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

        try
        {
            if (szConfigFileName != nullptr)
            {
                printf("[log4cplus]:load log config:%s\n",szConfigFileName);
                log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(szConfigFileName));
            }
        }
        catch(const std::exception& e)
        {
            printf("[log4cplus]:load log config: fail error msg: %s\n",e.what());
        }

        if (pList.size() > 1)
        {
            log4cplus::SharedAppenderPtrList::iterator it = pList.begin();
            printf("[log4cplus]:log4cplus has been initialized appenders size:%u\n", static_cast<uint32_t>(pList.size()));
            for (; it != pList.end(); ++it)
            {
                printf("Appender name:%s\n", it->get()->getName().c_str());
            }
            return true;
        }
        
        try
        {
            if (logger.getAllAppenders().size() == 0)
            {
                printf("[log4cplus]:set default log\n");
                logger.setLogLevel(log4cplus::ALL_LOG_LEVEL);

                // 设置TRACE~Debug信息日志格式 [zhuyb 2022-11-16 08:54:33]
                log4cplus::SharedAppenderPtr appendTraceToDebug(new log4cplus::ConsoleAppender());
                appendTraceToDebug->setName("traceToDebugConsole");
                std::string patternTraceToDebug = "\033[1;32m[D]<%-5p>[%D{%Y-%m-%d %H:%M:%S}][%b,%L][%t]:%m%n\033[0m";

                std::unique_ptr<log4cplus::Layout> layoutTraceToDebug(new log4cplus::PatternLayout(patternTraceToDebug));
                appendTraceToDebug->setLayout(std::move(layoutTraceToDebug));

                log4cplus::helpers::Properties proTrackToDebug;
                proTrackToDebug.setProperty("LogLevelMin", "TRACE");
                proTrackToDebug.setProperty("LogLevelMax", "DEBUG");

                log4cplus::spi::FilterPtr filterTraceToDebug(new log4cplus::spi::LogLevelRangeFilter(proTrackToDebug));
                appendTraceToDebug->addFilter(filterTraceToDebug);
                logger.addAppender(appendTraceToDebug);

                
                // 设置INFO信息日志格式 [zhuyb 2022-11-16 08:54:33]
                log4cplus::SharedAppenderPtr appendInfo(new log4cplus::ConsoleAppender());
                appendInfo->setName("infoConsole");
                std::string patternInfo = "[D]<%-5p>[%D{%Y-%m-%d %H:%M:%S}][%b,%L][%t]:%m%n";

                std::unique_ptr<log4cplus::Layout> layoutTraceToInfo(new log4cplus::PatternLayout(patternInfo));
                appendInfo->setLayout(std::move(layoutTraceToInfo));

                log4cplus::helpers::Properties proInfo;
                proInfo.setProperty("LogLevelMin", "INFO");
                proInfo.setProperty("LogLevelMax", "INFO");

                log4cplus::spi::FilterPtr filterInfo(new log4cplus::spi::LogLevelRangeFilter(proInfo));
                appendInfo->addFilter(filterInfo);
                // logger.setLogLevel(log4cplus::ALL_LOG_LEVEL);
                logger.addAppender(appendInfo);

                // 设置WARN~FATAL信息日志格式 [zhuyb 2022-11-16 08:54:45]
                log4cplus::SharedAppenderPtr appendWarnToFatal(new log4cplus::ConsoleAppender());
                appendWarnToFatal->setName("warnToFatalConsole");
                std::string patternWarnToFatal = "\033[1;31m[D]<%-5p>[%D{%Y-%m-%d %H:%M:%S}][%b,%L][%t]:%m%n\033[0m";
                // std::string patternWarnToFatal = "\033[1;31m[D]<%-5p>%n\033[0m";

                std::unique_ptr<log4cplus::Layout> layoutWarnToFatal(new log4cplus::PatternLayout(patternWarnToFatal));
                appendWarnToFatal->setLayout(std::move(layoutWarnToFatal));

                log4cplus::helpers::Properties proWarnToFatal;
                proWarnToFatal.setProperty("LogLevelMin", "WARN");
                proWarnToFatal.setProperty("LogLevelMax", "FATAL");
                
                log4cplus::spi::FilterPtr filterWarnToFatal(new log4cplus::spi::LogLevelRangeFilter(proWarnToFatal));
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

// #pragma GCC diagnostic pop