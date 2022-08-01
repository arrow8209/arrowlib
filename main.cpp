/*
 * @FilePath: /arrowlib/main.cpp
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-06-30 09:31:52
 * @Description:
 */
#include "pattern/factory/factory_test.h"
#include "pattern/observer/observer.h"
#include "pattern/observer/subject.h"
#include "pattern/singleton/singleton.h"
#include "task/task_one_thread.h"
#include <functional>
#include <stdio.h>
#include "typelist/typelist_demo.h"

// typedef IObserver<int, int, int> IIIObejct;
// typedef ISubject<int, int, int> IIISubject;

// class CObserver : public IIIObejct
// {
//     friend class Singleton<CObserver>;

// protected:
//     CObserver()
//     {
//     }

// public:
//     virtual int Response(int n1, int n2)
//     {
//         printf("1\n");
//         return 0;
//     }
// };
// typedef Singleton<CObserver> App_Observer;

// void fun1(void* param)
// {
//     printf("fun1\n");
// }

// constexpr static const int ntmp11= 0;

int main(int argc, char* argv[])
{
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

    test_typelist_type();
    // test_typelist_vlaue();
    test_static_string();
    // test_define();
    // Arrow::static_string::outinf("123456");


    return 1;
}