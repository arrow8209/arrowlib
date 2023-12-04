/*
 * @FilePath: /arrowlib/testmain/main.cpp
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-06-30 09:31:52
 * @Description:
 */
#include "arrow/arrow.h"
#include "arrow/log.h"

#include "demo/obj_pool/test_obj_pool1.h"

#include "demo/observer/test_observer1.h"
#include "demo/observer/test_observer2.h"

#include "demo/task/demo_task2.h"
#include "demo/task/test_task_performance.h"
#include "demo/task/test_lock_free_queue.h"

#include "demo/test_enum_name.h"
#include "demo/test_type_name.h"

#include "demo/factory/factory_demo.h"

#include "demo/state_demo/state_demo.h"

#include "demo/test_file_name.h"

int main(int argc, char* argv[])
{
    ArrowLog::init(nullptr);
    ARROW_LOG_TRACE("TRACE");
    ARROW_LOG_DEBUG("DEBUG");
    ARROW_LOG_INFO("INFO");
    ARROW_LOG_WARN("WARN");
    ARROW_LOG_ERROR("ERROR");
    // TestObserver1();
    // TestObserver2();
    // TestObjPool1();
    TestObjPool2();
    // DemoTask2();
    // TestTaskPerformance();
    // TestTak3();
    // TestLockFree();
    // TestStaticStr();
    // TestTypeName1();
    // TestTypeName2();
    // TestStrDjb2();
    // TestFileName();
    // TestFunName();
    // RunTestTask();
    // TestEnum();
    
    // MainTestArgs();
    // TestLockFree1();
    // TestTask();
    // TestMapView();
    // TestLogOut();
    // TestSetTupe();
    // TestTask1();

    // TestEnumName();

    // FactoryDemo();

    // StateDemo();
    return 1;
}
