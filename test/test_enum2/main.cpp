/**
 * @file main.cpp
 * @brief 测试枚举到字符串转换功能的主程序
 */

#include <iostream>
#include "test_enum_to_string.h"

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

// 添加以下函数在main函数中调用
void print_pretty_function_debug() {
    std::cout << "DEBUG - EMC函数签名: " << __PRETTY_FUNCTION__ << std::endl;
    
    // 为EMC类型调用debug_print_pretty_function
    enum_utils::debug_print_function(__PRETTY_FUNCTION__);
    
    // 测试EMC类型的get_enum_type_prefix
    std::cout << "EMC类型前缀: " << enum_utils::get_enum_type_prefix<EMC>().data() << std::endl;
}

int main() {
    // 打印编译器信息
    print_compiler_info();
    
    std::cout << "\n===== 枚举到字符串转换测试 =====" << std::endl;
    
    std::cout << "打印调试信息..." << std::endl;
    test_enum::debug_print_pretty_function<EMA, EMA::EMA1>();
    test_enum::debug_print_pretty_function<EMC, EMCA1>();
    
    // 添加EMC类型的详细调试
    print_pretty_function_debug();
    
    // 运行测试
    test_enum::run_test();
    
    return 0;
} 