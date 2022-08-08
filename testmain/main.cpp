/*
 * @FilePath: /arrowlib/testmain/main.cpp
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-06-30 09:31:52
 * @Description:
 */
#include <functional>
#include <stdio.h>
#include <thread>
#include "arrow/pattern/factory/factory_test.h"
#include "arrow/pattern/observer/observer.h"
#include "arrow/pattern/observer/subject.h"
#include "arrow/pattern/singleton/singleton.h"
#include "arrow/task/task_one_thread.h"
#include "arrow/typelist/typelist_demo.h"
#include "arrow/log/log.h"
#include "arrow/log/log_impl_log4cplus.h"
#include "arrow/other/load_lib_test.h"
#include <log4cplus/log4cplus.h>
 typedef Arrow::Log::LogInterface<Arrow::Log::LogImplLog4Cplus> ALog;

void fun()
{

}

constexpr static char const dataX[] = "1213";
constexpr static char const* data1 = dataX;

int main(int argc, char* argv[])
{
    std::cout << dataX << std::endl;
    std::cout << data1 << std::endl;
    printf("\nLD_LIBRARY_PATH:\n");
    system("echo $LD_LIBRARY_PATH");
    
    // std::thread th = std::thread(&fun); 
    
    ALog::log_init(nullptr);
    ALog::Trace_Log<STATIC_FILE, STATIC_FUNC, __LINE__>("12312");
    ALog::Trace_Log<STATIC_FILE, STATIC_FUNC, __LINE__>("A:%s", "12312");
    ARROW_LOG_TRACE("12312");
    // while(true)
    // {
    //     printf("asdfas\n");
    // }

    // App_Observer::Instance()->Response(1, 2);
    // App_Observer::instance_()->Printf();

    // CTestTask testtask;
    // testtask.InitTask();
    // testtask.StartTask();
    // testtask.TstAddTask();
    // int n1;
    // std::cin >> n1;
    // testtask.EndTask();

    // App_Factory::Create(3);
    // App_Factory::Create(1);
    // App_Factory::Create(2);
    // App_Factory::Create(4);
    
    // test_typelist_type();
    // test_typelist_vlaue();
    // test_static_string();
    // test_define();
    // Arrow::static_string::outinf("123456");
    // test_static_string2();
    // log4cplus::initialize();

    // Test_Load_Lib();
    TestPair();


    // if (th.joinable())
    // {
    //     th.join();
    // }
    
    return 1;
}