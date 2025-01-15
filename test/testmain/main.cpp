/*
 * @FilePath: /arrowlib/test/testmain/main.cpp
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-06-30 09:31:52
 * @Description:
 */
#include <functional>
#include <stdio.h>
#include <thread>
#include <type_traits>
#include <boost/filesystem.hpp>

#include "arrow/pattern/pattern.h"
#include "arrow/task/task.h"
#include "arrow/log.h"

#include "demo/state_demo/state_demo_2.h"
#include "demo/test_enum_name.h"
#include "demo/responsibility/responsibility_demo1.h"
#include "demo/test_load_lib.h"

int main(int argc, char* argv[])
{
    std::string strTmp2 = boost::filesystem::current_path().string();
    std::cout << strTmp2 << std::endl;
    // ArrowLog::init(nullptr);
    // TestEnumName();
    // State_Demo_2_H::StateDemo();
    // TestResponsibility1();
    int* p = (int*)malloc(sizeof(int));
    TestLoadLib();
    
    return 1;
}
