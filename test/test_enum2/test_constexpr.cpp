/**
 * @file test_constexpr.cpp
 * @brief 测试string_view的find函数是否在编译期执行
 */

#include "enum_to_string.h"
#include <iostream>

using namespace enum_utils;

// 测试函数是否在编译期执行
constexpr bool test_find_at_compile_time() {
    string_view s1("hello world");
    string_view s2("world");
    auto pos = s1.find(s2);
    return pos == 6; // "world"在"hello world"中的位置是6
}

// 编译期断言 - 如果find不是在编译期执行，这里会编译失败
static_assert(test_find_at_compile_time(), "find() 没有在编译期正确执行");

int main() {
    std::cout << "如果程序编译成功并运行到这里，说明find()函数确实在编译期执行了" << std::endl;
    
    // 运行时测试
    string_view s1("hello world");
    string_view s2("world");
    auto pos = s1.find(s2);
    
    std::cout << "在运行时，\"" << s2.data() << "\" 在 \"" << s1.data() 
              << "\" 中的位置是: " << pos << std::endl;
    
    return 0;
} 