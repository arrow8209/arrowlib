#pragma once
#include "arrow/other/load_lib.h"
#include "arrow/other/std_assist.h"

static void TestLogInLib()
{
    Arrow::Other::DynamicLib testlib1;
    Arrow::Other::DynamicLib testlib2;
    testlib1.LoadLib("libtestlib1.so");
    testlib2.LoadLib("libtestlib2.so");
    if (testlib1.CheckOpenLib() == false || testlib2.CheckOpenLib() == false)
    {
        std::cout<< "123" << testlib1.LastErrorMsg() << std::endl;
        return;
    }

    std::string strPath = Arrow::Other::get_app_path();
    try
    {
        testlib1.call<Arrow::Other::_stdcall_, uint32_t>("Init", strPath.c_str());
        testlib2.call<Arrow::Other::_stdcall_,uint32_t>("Init", strPath.c_str());
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    testlib1.CloseLib();
    testlib2.CloseLib();
    std::cout << "Test_Load_Lib end" << std::endl;
}