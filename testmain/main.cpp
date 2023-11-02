/*
 * @FilePath: /arrowlib/testmain/main.cpp
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-06-30 09:31:52
 * @Description:
 */
#include "arrow/arrow.h"

#include "demo/obj_pool/test_obj_pool1.h"

#include "demo/observer/test_observer1.h"
#include "demo/observer/test_observer2.h"

#include "demo/task/demo_task2.h"
#include "demo/task/test_task_performance.h"
#include "demo/task/test_lock_free_queue.h"

int main(int argc, char* argv[])
{
    // TestObserver1();
    // TestObserver2();
    // TestObjPool1();
    DemoTask2();
    // TestTaskPerformance();
    // TestTak3();
    // TestLockFree();
    // TestStaticStr();
    // TestTypeName();
    // TestEnumName();
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
    return 1;
}
