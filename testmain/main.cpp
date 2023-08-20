/*
 * @FilePath: /arrowlib/testmain/main.cpp
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-06-30 09:31:52
 * @Description:
 */

#include "demo/test_static_str.h"
#include "demo/test_type_name.h"
#include "demo/test_file_name.h"
#include "demo/test_func_name.h"
#include "demo/test_task.h"

#include "../test_enum/test_enum.h"
#include "../test_enum/test1/map_view.h"

int main(int argc, char* argv[])
{
    // TestStaticStr();
    // TestTypeName();
    // TestEnumName();
    // TestStrDjb2();
    // TestFileName();
    // TestFunName();
    // RunTestTask();
    TestEnum();

    // TestMapView();
    return 1;
}
