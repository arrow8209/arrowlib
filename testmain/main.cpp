/*
 * @FilePath: /arrowlib/testmain/main.cpp
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-06-30 09:31:52
 * @Description:
 */
#include <functional>
#include <stdio.h>
#include <thread>
#include <type_traits>
// #include "arrow/pattern/factory/factory_test.h"
#include "arrow/pattern/observer/observer.h"
#include "arrow/pattern/observer/subject.h"
#include "arrow/pattern/singleton/singleton.h"
#include "arrow/task/task_one_thread.h"
#include "demo/typelist_demo.h"
#include "arrow/log/log.h"
// #include "arrow/log/log_impl_log4cplus.h"
// #include "arrow/other/load_lib_test.h"
#include "test_log_in_lib.h"
// #include <log4cplus/log4cplus.h>
//  typedef Arrow::Log::LogInterface<Arrow::Log::LogImplLog4Cplus> ALog;

#include "demo/task_test.h"

// #include "test_enum_to_str.h"
#include "test_enum_to_str2.h"
#include "test_static_map.h"
#include "demo/static_str2.h"
typedef Arrow::TLog<Arrow::Log::LogImplDefault> Log;

void CPlustThreadInit()
{
    std::cout << "CPlustThreadInit" << std::endl;
}

template<bool value>
using MyConditional = typename std::conditional<value, int, float>::type;

int main(int argc, char* argv[])
{
    // log4cplus::initialize();
    // std::thread cplusThreadInit = std::thread(&CPlustThreadInit);
    // if (cplusThreadInit.joinable())
    // {
    //     cplusThreadInit.join();
    // }
    
    // printf("\nLD_LIBRARY_PATH:\n");
    // system("echo $LD_LIBRARY_PATH");

    // Log::Log<Log_Param(Arrow::Log_Debug)>(56789);
    

    // TestLogInLib();

    // log4cplus::initialize();
    // std::thread th = std::thread(&fun); 
    
    // ALog::init(nullptr);
    // ALog::Trace_Log<STATIC_FILE, STATIC_FUNC, __LINE__>("12312");
    // ALog::Trace_Log<STATIC_FILE, STATIC_FUNC, __LINE__>("A:%s", "12312");
    // ARROW_LOG_TRACE("12312");
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
    // TestIntSquence();
    // TestStaticSubStr();
    // TestStringView();
    TestStrView2();
    // TestTypeName();
    // Arrow::static_string::outinf("123456");
    // test_static_string2();
    // log4cplus::initialize();

    // Test_Load_Lib();
    // TestPair();


    // if (th.joinable())
    // {
    //     th.join();
    // }

    // test_static_string2();

    // using bValue = std::true_type::value;

    // TestPair2();
    // CTestTask taskTestmp;
    // taskTestmp.Activate();
    // taskTestmp.TstAddTask();

    // int nTmp; 
    // std::cin >>  nTmp;
    // TestEnumToStr();
    // TestEnum2();
    // std::cout << __cplusplus << std::endl;
    // TestStaticMap();

    // std::cout << typeid(MyConditional<true>).name() << std::endl;
    // std::cout << typeid(MyConditional<false>).name() << std::endl;
    return 1;
}
