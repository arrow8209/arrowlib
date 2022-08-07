#pragma once

#include "std_assist.h"
#include "load_lib.h"

static void Test_Load_Lib()
{
    Arrow::Other::DynamicLib testlib1;
    try
    {
        testlib1.LoadLib("libtestlib1.so");
        std::string strPath = Arrow::Other::get_app_path();

        testlib1.call<uint32_t>("Init", strPath.c_str());
        std::cout << "run init 2" << std::endl;
        testlib1.CloseLib();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    std::cout << "Test_Load_Lib end" << std::endl;
    
}