/**
 * @file test_enum_to_string.h
 * @brief 测试枚举到字符串转换功能
 */

#pragma once

#include <iostream>
#include <string>
#include "enum_to_string.h"

// 测试枚举类型
enum class EMA
{
    EMA1 = 0,
    EMA2,
    EMA3
};

// 带有不连续值的枚举类型
enum class EMB
{
    EMB1 = 100,
    EMB2 = 104,
    EMB3 = 110
};

// 带有多个范围的枚举类型
enum EMC
{
    EMCA1 = 0,
    EMCA2,
    EMCA3,
    EMCB1 = 100,
    EMCB2,
    EMCB3,
    EMCC1 = 200,
    EMCC2,
    EMCC3
};

// 定义每种枚举类型对应的字符串转换
namespace test_enum {

// 调试函数，打印__PRETTY_FUNCTION__的内容
template<typename EnumType, EnumType E>
void debug_print_pretty_function() {
    std::cout << "DEBUG [" << __PRETTY_FUNCTION__ << "]" << std::endl;
}

// EMA的字符串转换
using EMAString = enum_utils::enum_to_string<
    EMA,
    EMA::EMA1,
    EMA::EMA2,
    EMA::EMA3
>;

// EMB的字符串转换
using EMBString = enum_utils::enum_to_string<
    EMB,
    EMB::EMB1,
    EMB::EMB2,
    EMB::EMB3
>;

// EMC的字符串转换
using EMCString = enum_utils::enum_to_string<
    EMC,
    EMC::EMCA1, EMC::EMCA2, EMC::EMCA3,
    EMC::EMCB1, EMC::EMCB2, EMC::EMCB3,
    EMC::EMCC1, EMC::EMCC2, EMC::EMCC3
>;

// 运行测试函数
inline void run_test() {
    std::cout << "===== 枚举到字符串转换测试 =====" << std::endl;
    
    // 打印DEBUG信息
    std::cout << "打印调试信息..." << std::endl;
    debug_print_pretty_function<EMA, EMA::EMA1>();
    debug_print_pretty_function<EMC, EMC::EMCA1>();

    // 测试EMA
    std::cout << "\n== EMA 枚举测试 ==" << std::endl;
    std::cout << "完整名称: " << EMAString::Str(EMA::EMA1) << std::endl;
    std::cout << "短名称: " << EMAString::ShortStr(EMA::EMA1) << std::endl;
    std::cout << "完整名称: " << EMAString::Str(EMA::EMA2) << std::endl;
    std::cout << "短名称: " << EMAString::ShortStr(EMA::EMA2) << std::endl;
    
    // 测试EMB (不连续值)
    std::cout << "\n== EMB 枚举测试（不连续值）==" << std::endl;
    std::cout << "完整名称: " << EMBString::Str(EMB::EMB1) << std::endl;
    std::cout << "短名称: " << EMBString::ShortStr(EMB::EMB1) << std::endl;
    std::cout << "完整名称: " << EMBString::Str(EMB::EMB2) << std::endl;
    std::cout << "短名称: " << EMBString::ShortStr(EMB::EMB2) << std::endl;
    
    // 测试EMC (多个范围)
    std::cout << "\n== EMC 枚举测试（多个范围）==" << std::endl;
    std::cout << "完整名称: " << EMCString::Str(EMC::EMCA1) << std::endl;
    std::cout << "短名称: " << EMCString::ShortStr(EMC::EMCA1) << std::endl;
    std::cout << "完整名称: " << EMCString::Str(EMC::EMCB1) << std::endl;
    std::cout << "短名称: " << EMCString::ShortStr(EMC::EMCB1) << std::endl;
    
    // 测试不存在的枚举值
    std::cout << "\n== 测试不存在的枚举值 ==" << std::endl;
    EMC invalid_enum = static_cast<EMC>(999);
    std::cout << "完整名称: " << EMCString::Str(invalid_enum) << std::endl;
    std::cout << "EMC(999) 是否存在: " << (EMCString::Contains(invalid_enum) ? "是" : "否") << std::endl;
    
    std::cout << "\n===== 测试完成 =====" << std::endl;
}

} // namespace test_enum 