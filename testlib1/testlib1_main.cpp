#include <stdio.h>
#include <stdint.h>
// #include <log4cplus/log4cplus.h>
#include <iostream>
#include "arrow/log/log.h"
#include "arrow/log/log_impl_log4cplus.h";

typedef Arrow::Log::base_log<Arrow::Log::LogImplLog4Cplus> ALog2;

// #include "zhjyz.h"
// #include "application.h"

#ifdef __cplusplus
extern "C"
{
#endif


    int main(int argc, char* argv[])
    {
        // return App_Zhjyz::Instance()->Init(szConfigPath);
        // printf(szConfigPath);
        ALog2::init("xdfstestlib1.log.ini");
        ALog2::Log<Log_Param(Arrow::Log_Debug)>("testlib1");
        // ARROW_LOG_DEBUG("test lib1");
        // std::cout << szConfigPath << std::endl;
        std::cout << "run init" << std::endl;
        // log4cplus::initialize();
        return 0;
    }

#ifdef __cplusplus
}
#endif


__attribute ((constructor)) void testlib1_init(void)
{
    printf("* %s\n", __func__);
}

__attribute ((destructor)) void testlib1_fini(void)
{
    printf("* %s\n", __func__);
}