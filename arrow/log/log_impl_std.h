#pragma once
#include "log_interface.h"
#include "../typelist/typelist.h"

namespace Arrow
{

namespace Log
{
namespace details_std
{
    typedef  Arrow::static_map<
     Arrow::static_pair<Arrow::value_type<LogNull>,STATIC_STRING("Null")>,
     Arrow::static_pair<Arrow::value_type<LogTrace>,STATIC_STRING("Trace")>,
     Arrow::static_pair<Arrow::value_type<LogDebug>,STATIC_STRING("Debug")>,
     Arrow::static_pair<Arrow::value_type<LogInfo>,STATIC_STRING("Info")>,
     Arrow::static_pair<Arrow::value_type<LogWarn>,STATIC_STRING("Warn")>,
     Arrow::static_pair<Arrow::value_type<LogError>,STATIC_STRING("Error")>,
     Arrow::static_pair<Arrow::value_type<LogFatal>,STATIC_STRING("Fatal")>,
     Arrow::static_pair<Arrow::value_type<LogMax>,STATIC_STRING("Max")>
     > LogTypeStr;
}

class LogImplDefault
{
protected:
    LogImplDefault() = default;
    LogImplDefault(LogImplDefault& t) = delete;
    LogImplDefault& operator=(const LogImplDefault& t) = delete;
    virtual ~LogImplDefault() = default;

public:
    template<typename ...Args>
    static bool init(Args... args)
    {
        return true;
    }

    template <EmLogLevel loglevel, typename TFileName, typename TFunName, int line, typename T>
    static void Log(const T& t)
    {
        Prefix<loglevel, TFileName, TFunName, line>();
        std::cout << t << std::endl;
    }

    template <EmLogLevel loglevel, typename TFileName, typename TFunName, int line, typename TFmt,  typename... Args>
    static void Log(Args... args)
    {
        Prefix<loglevel, TFileName, TFunName, line>();
        printf(tlist::tvaluelist_to_data<TFmt>::data, args...);
    }

protected:
    template <EmLogLevel loglevel, typename TFileName, typename TFunName, int line>    
    static void Prefix()
    {
        // 输出格式 [logvele][filename:line] [zhuyb 2022-08-12 09:42:38]
        // std::cout<< "[" << Arrow::smap::get<Arrow::value_type<loglevel>, details_std::LogTypeStr>::Pair::value << "]"
        std::cout<< "[" << details_std::LogTypeStr::get<Arrow::value_type<loglevel>>::value << "]"
                    << "[" << tlist::tvaluelist_to_data<TFileName>::data << ":" << line << "]"
                    << "[" << tlist::tvaluelist_to_data<TFunName>::data << "]";
    }
};

} // namespace Log
} // namespace Arrow