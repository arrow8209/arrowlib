#pragma once
#include "log_interface.h"

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

    // template <EmLogLevel loglevel, typename TFileName, typename TFunName, int line, typename T>
    // static void Log(const T& t)
    // {
    // }

    // template <EmLogLevel loglevel, typename TFileName, typename TFunName, int line, typename TFmt, typename... Args>
    // static void Log(Args... args)
    // {
    // }


};

} // namespace Log
} // namespace Arrow