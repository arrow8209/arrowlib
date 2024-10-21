#pragma once
#include "arrow/arrow.h"
#include "arrow/log.h"
    

class OutGstInfo
{
public:
    template<Arrow::EmLogLevel _logEvel>
    static void Out()
    {
            ArrowLog::Log<LOG_PARAM(Arrow::EmLogLevel::LogWarn)>("caps is null ");
    }
};

void TestLogOut()
{
    ArrowLog::init(nullptr);

    ARROW_LOG_TRACE("TRACE");
    ARROW_LOG_DEBUG("DEBUG");
    ARROW_LOG_INFO("INFO");
    ARROW_LOG_WARN("WARN");
    ARROW_LOG_ERROR("ERROR");

    typedef STATIC_STRING("%s %10s:(%s)%s ") TMP;
    std::cout << Arrow::ValueListToData<TMP>::data << std::endl;
    OutGstInfo::Out<Arrow::EmLogLevel::LogWarn>();
    // OutGstInfo::Out();
    // OutGstInfo<Arrow::EmLogLevel::LogWarn>::Out();
    ArrowLog::Log<LOG_PARAM(Arrow::EmLogLevel::LogWarn)>("caps is null ");

    typedef STATIC_STRING(__FILE__) MyFileName1;
    std::cout << Arrow::ValueListToData<MyFileName1>::data << std::endl;

    typedef Arrow::Splite<Arrow::StaticStr::FindLast(__FILE__, '/') + 1, STATIC_STRING(__FILE__)>::Tail MyFileNam2;
    std::cout << Arrow::ValueListToData<MyFileNam2>::data << std::endl;

    // using MyLog4Type = Arrow::static_map<
    //     Arrow::static_pair<Arrow::IntValueType<1>, Arrow::IntValueType<log4cplus::TRACE_LOG_LEVEL>>,
    //     Arrow::static_pair<Arrow::IntValueType<2>, Arrow::IntValueType<log4cplus::TRACE_LOG_LEVEL>>,
    //     Arrow::static_pair<Arrow::IntValueType<3>, Arrow::IntValueType<log4cplus::DEBUG_LOG_LEVEL>>,
    //     Arrow::static_pair<Arrow::IntValueType<4>, Arrow::IntValueType<log4cplus::INFO_LOG_LEVEL>>,
    //     Arrow::static_pair<Arrow::IntValueType<5>, Arrow::IntValueType<log4cplus::WARN_LOG_LEVEL>>,
    //     Arrow::static_pair<Arrow::IntValueType<6>, Arrow::IntValueType<log4cplus::ERROR_LOG_LEVEL>>,
    //     Arrow::static_pair<Arrow::IntValueType<7>, Arrow::IntValueType<log4cplus::FATAL_LOG_LEVEL>>,
    //     Arrow::static_pair<Arrow::IntValueType<8>, Arrow::IntValueType<log4cplus::TRACE_LOG_LEVEL>>>;
    // std::cout << MyLog4Type::get<Arrow::IntValueType<1>>::value <<std::endl;


    std::cout << GET_FILE_NAME("../ShareCode/gstcpppackage/gstassist/out_gst_info.h").data << std::endl;
    typedef STATIC_STRING((GET_FILE_NAME("../ShareCode/gstcpppackage/gstassist/out_gst_info.h").data)) MyFileName;
    // ArrowLog::Log<LOG_PARAM_FMT(Arrow::EmLogLevel::LogWarn, "%s %10s:(%s)%s ")>("789", "123", "456", "abc");
    // ArrowLog::Log<LOG_PARAM(Arrow::EmLogLevel::LogDebug)>("caps is null ");
    // ArrowLog::Log<LOG_PARAM(_logEvel)>("caps is null ");
}