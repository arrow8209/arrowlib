#pragma once
#include "../typelist/typelist.h"

namespace Arrow
{

namespace Log
{
namespace details_std
{
    typedef  Arrow::static_map<
     Arrow::static_pair<Arrow::value_type<Log_Null>,STATIC_STRING("Null")>,
     Arrow::static_pair<Arrow::value_type<Log_Trace>,STATIC_STRING("Trace")>,
     Arrow::static_pair<Arrow::value_type<Log_Debug>,STATIC_STRING("Debug")>,
     Arrow::static_pair<Arrow::value_type<Log_Info>,STATIC_STRING("Info")>,
     Arrow::static_pair<Arrow::value_type<Log_Warn>,STATIC_STRING("Warn")>,
     Arrow::static_pair<Arrow::value_type<Log_Error>,STATIC_STRING("Error")>,
     Arrow::static_pair<Arrow::value_type<Log_Fatal>,STATIC_STRING("Fatal")>,
     Arrow::static_pair<Arrow::value_type<Log_Max>,STATIC_STRING("Max")>
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

    template <Em_Log_Level loglevel, typename TFileName, typename TFunName, int line, typename T>
    static void Log(const T& t)
    {
        Prefix<loglevel, TFileName, TFunName, line>();
        std::cout << t << std::endl;
    }

    template <Em_Log_Level loglevel, typename TFileName, typename TFunName, int line, typename... Args>
    static void Log(const char* szFmt, Args... args)
    {
        Prefix<loglevel, TFileName, TFunName, line>();
        printf(szFmt, args...);
    }

protected:
    template <Em_Log_Level loglevel, typename TFileName, typename TFunName, int line>    
    static void Prefix()
    {
        // 输出格式 [logvele][filename:line] [zhuyb 2022-08-12 09:42:38]
        std::cout<< "[" << Arrow::smap::get<Arrow::value_type<loglevel>, details_std::LogTypeStr>::Pair::value << "]"
                    << "[" << tlist::tvaluelist_to_data<TFileName>::data << ":" << line << "]"
                    << "[" << tlist::tvaluelist_to_data<TFunName>::data << "]";
    }
};

} // namespace Log
} // namespace Arrow