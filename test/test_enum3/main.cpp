/**
 * @file main.cpp
 * @brief 测试自动枚举检测功能的主程序
 */

#include <iostream>
#include "test_enum_to_string_auto.h"

// 一个辅助函数，在调试时输出当前编译器类型
void print_compiler_info() {
    std::cout << "========= 编译器信息 =========" << std::endl;
    #if defined(__clang__)
        std::cout << "编译器: Clang " << __clang_major__ << "." << __clang_minor__ << std::endl;
    #elif defined(__GNUC__) || defined(__GNUG__)
        std::cout << "编译器: GCC " << __GNUC__ << "." << __GNUC_MINOR__ << std::endl;
    #elif defined(_MSC_VER)
        std::cout << "编译器: MSVC " << _MSC_VER << std::endl;
    #else
        std::cout << "编译器: 未知" << std::endl;
    #endif
    
    std::cout << "C++ 标准: ";
    #if __cplusplus >= 201703L
        std::cout << "C++17 或更高" << std::endl;
    #elif __cplusplus >= 201402L
        std::cout << "C++14" << std::endl;
    #elif __cplusplus >= 201103L
        std::cout << "C++11" << std::endl;
    #else
        std::cout << "C++98 或更早" << std::endl;
    #endif
    std::cout << "=============================\n" << std::endl;
}

int main() {
    // 打印编译器信息
    print_compiler_info();
    
    std::cout << "\n===== 自动枚举检测功能测试 =====" << std::endl;
    
    // 运行测试
    test_enum_auto::run_test();
    
    return 0;
} 