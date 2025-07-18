/**
 * @file test_const_char.cpp
 * @brief 测试 const char* 返回值
 */

#include <iostream>
#include <cstring>
#include "enum_to_string_auto.h"
#include "test_enum_to_string_auto.h"

int main() {
    std::cout << "===== 测试 const char* 返回值 =====" << std::endl;
    
    // 测试 Str 方法
    const char* full_name = test_enum_auto::EMDString::Str(EMD::EMDA1);
    std::cout << "EMD::EMDA1 完整名称: " << full_name << std::endl;
    std::cout << "字符串长度: " << strlen(full_name) << std::endl;
    std::cout << "地址: " << static_cast<const void*>(full_name) << std::endl;
    
    // 测试 ShortStr 方法
    const char* short_name = test_enum_auto::EMDString::ShortStr(EMD::EMDA1);
    std::cout << "EMD::EMDA1 短名称: " << short_name << std::endl;
    std::cout << "字符串长度: " << strlen(short_name) << std::endl;
    std::cout << "地址: " << static_cast<const void*>(short_name) << std::endl;
    
    // 测试枚举类
    const char* enum_class_name = test_enum_auto::EMEString::Str(EME::EME1);
    std::cout << "EME::EME1 完整名称: " << enum_class_name << std::endl;
    std::cout << "字符串长度: " << strlen(enum_class_name) << std::endl;
    std::cout << "地址: " << static_cast<const void*>(enum_class_name) << std::endl;
    
    // 测试不存在的枚举值
    const char* unknown_name = test_enum_auto::EMDString::Str(static_cast<EMD>(999));
    std::cout << "EMD(999) 名称: " << unknown_name << std::endl;
    std::cout << "字符串长度: " << strlen(unknown_name) << std::endl;
    
    std::cout << "===== 测试完成 =====" << std::endl;
    return 0;
}