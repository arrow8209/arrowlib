/**
 * @file debug_pretty_function.cpp
 * @brief 用于调试__PRETTY_FUNCTION__的实际输出格式
 */

#include <iostream>
#include <string_view>

// 测试枚举类型
enum class EMA {
    EMA1 = 1,
    EMA2 = 2,
    EMA3 = 3
};

// 打印函数签名的函数模板
template <typename EnumType, EnumType E>
void print_function_signature() {
    std::cout << "函数签名: " << __PRETTY_FUNCTION__ << std::endl;
}

// 测试不同编译器下的__PRETTY_FUNCTION__格式
int main() {
    std::cout << "===== __PRETTY_FUNCTION__ 格式测试 =====" << std::endl;
    
    // 打印编译器信息
    std::cout << "编译器: ";
#if defined(__clang__)
    std::cout << "Clang " << __clang_major__ << "." << __clang_minor__ << std::endl;
#elif defined(__GNUC__) || defined(__GNUG__)
    std::cout << "GCC " << __GNUC__ << "." << __GNUC_MINOR__ << std::endl;
#elif defined(_MSC_VER)
    std::cout << "MSVC " << _MSC_VER << std::endl;
#else
    std::cout << "未知编译器" << std::endl;
#endif

    std::cout << "C++ 标准: ";
#if __cplusplus >= 201703L
    std::cout << "C++17 或更高" << std::endl;
#else
    std::cout << "低于 C++17" << std::endl;
#endif

    // 测试枚举值的函数签名
    print_function_signature<EMA, EMA::EMA1>();
    
    return 0;
} 