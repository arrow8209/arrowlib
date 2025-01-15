#include <stdio.h>
#include <stdint.h>
// #include <log4cplus/log4cplus.h>
#include <iostream>
#include "arrow/arrow.h"


// #include "zhjyz.h"
// #include "application.h"

#ifdef __cplusplus
extern "C"
{
#endif

    /**
     * @description: 初始化模块 可多次调用，但是只有第一次调用的szConfigPath有效
     * @param {char*} szConfigPath 配置文件目录
     * @return {*} 0 成功 其他 错误码
     */
    ARROW_API_EXPORT uint32_t Init(const char* szConfigPath)
    {
        // return App_Zhjyz::Instance()->Init(szConfigPath);
        // printf(szConfigPath);
        // ALog::init("xdfstestlib1.log.ini");
        // ALog::Log<LOG_PARAM(Arrow::LogDebug)>("testlib1");
        // ARROW_LOG_DEBUG("test lib1");
        // std::cout << szConfigPath << std::endl;
        std::cout << "run init" << std::endl;
        // log4cplus::initialize();
        return 0;
    }

#ifdef __cplusplus
}
#endif


// __attribute ((constructor)) void testlib1_init(void)
// {
//     printf("* %s\n", __func__);
// }

// __attribute ((destructor)) void testlib1_fini(void)
// {
//     printf("* %s\n", __func__);
// }