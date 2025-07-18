/**
 * @file test_enum_to_string_auto.h
 * @brief 测试自动枚举检测功能
 */

#pragma once

#include <iostream>
#include <string>
#include "enum_to_string_auto.h"

// 测试枚举类型
enum EMD
{
    EMDA1 = 0,
    EMDA2,
    EMDA3,
    EMDB1 = 100,
    EMDB2,
    EMDB3,
    EMDC1 = 200,
    EMDC2,
    EMDC3
};

// 简单枚举类型
enum class EME
{
    EME1 = 0,
    EME2,
    EME3
};

// 普通枚举类型
enum EMF
{
    EMFA1 = 0,
    EMFA2,
    EMFA3,
    EMFB1 = 125,
    EMFB2,
    EMFB3
};

namespace test_enum_auto {

// 使用自动检测功能的枚举字符串转换
// 只需提供枚举类型和起始值，库会自动检测同一范围内的所有枚举值
using EMDString = enum_utils::enum_to_string<
    EMD,
    EMD::EMDA1, // 提供第一个范围的起始值
    EMD::EMDB1, // 提供第二个范围的起始值
    EMD::EMDC1  // 提供第三个范围的起始值
>;

// 简单枚举类的字符串转换，只提供一个起始值
using EMEString = enum_utils::enum_to_string<
    EME,
    EME::EME1   // 只提供一个起始值，会自动检测EME1、EME2、EME3
>;

// 普通枚举的字符串转换
using EMFString = enum_utils::enum_to_string<
    EMF,
    EMF::EMFA1, // 提供第一个范围的起始值
    EMF::EMFB1  // 提供第二个范围的起始值
>;
template<typename T, T... ints>
void print_index_sequence(std::integer_sequence<T, ints...> int_seq)
{
    std::cout << "The sequence of size " << int_seq.size() << ": ";
    ((std::cout << ints << ' '),...);
    std::cout << '\n';
}

// 运行测试函数
inline void run_test() {

    enum_utils::debug_print_pretty_function<EMD, EMD::EMDA1>();
    enum_utils::debug_print_pretty_function<EMD, static_cast<EMD>(19)>();
    enum_utils::debug_print_pretty_function<EMD, static_cast<EMD>(255)>();
    enum_utils::debug_print_pretty_function<EME, EME::EME1>();
    enum_utils::debug_print_pretty_function<EME, static_cast<EME>(19)>();
    enum_utils::debug_print_pretty_function<EME, static_cast<EME>(255)>();

    std::cout << "===== 枚举检测验证 =====" << std::endl;

    // EMDString::Debug(EMD::EMDA1);
    // EMEString::Debug(EME::EME1);
    // EMFString::Debug(EMF::EMFA1);

    // 测试有效值
    std::cout << "有效枚举值EMD::EMDA1检测: " << (enum_utils::is_valid_enum_value<EMD, EMD::EMDA1, 0>::value ? "有效" : "无效") << std::endl;
    std::cout << "有效枚举值EMD::EMDA2检测: " << (enum_utils::is_valid_enum_value<EMD, EMD::EMDA1, 1>::value ? "有效" : "无效") << std::endl;
    
    // 测试无效值
    std::cout << "无效枚举值EMD(255)检测: " << (enum_utils::is_valid_enum_value<EMD, EMD::EMDA1, 255>::value ? "有效" : "无效") << std::endl;
    std::cout << "无效枚举值EMF(127)检测: " << (enum_utils::is_valid_enum_value<EMF, EMF::EMFA1, 127>::value ? "有效" : "无效") << std::endl;
    std::cout << "无效枚举值EME(127)检测: " << (enum_utils::is_valid_enum_value<EME, EME::EME1, 127>::value ? "有效" : "无效") << std::endl;
    
    // 输出枚举名称
    std::cout << "EMD(255)名称: " << enum_utils::get_enum_name_impl<EMD, static_cast<EMD>(255)>().data() << std::endl;
    std::cout << "EMF(127)名称: " << enum_utils::get_enum_name_impl<EMF, static_cast<EMF>(127)>().data() << std::endl;
    std::cout << "EME(127)名称: " << enum_utils::get_enum_name_impl<EME, static_cast<EME>(127)>().data() << std::endl;

    std::cout << "===== 自动枚举检测转换测试 =====" << std::endl;
    
    // 测试EMD (使用自动检测的多个范围)
    std::cout << "\n== EMD 枚举测试 (自动检测多个范围) ==" << std::endl;
    std::cout << "起始值 EMDA1: " << EMDString::FullStr(EMD::EMDA1) << std::endl;
    std::cout << "自动检测 EMDA2: " << EMDString::FullStr(EMD::EMDA2) << std::endl;
    std::cout << "自动检测 EMDA3: " << EMDString::FullStr(EMD::EMDA3) << std::endl;
    
    std::cout << "起始值 EMDB1: " << EMDString::FullStr(EMD::EMDB1) << std::endl;
    std::cout << "自动检测 EMDB2: " << EMDString::FullStr(EMD::EMDB2) << std::endl;
    std::cout << "自动检测 EMDB3: " << EMDString::FullStr(EMD::EMDB3) << std::endl;
    
    std::cout << "起始值 EMDC1: " << EMDString::FullStr(EMD::EMDC1) << std::endl;
    std::cout << "自动检测 EMDC2: " << EMDString::FullStr(EMD::EMDC2) << std::endl;
    std::cout << "自动检测 EMDC3: " << EMDString::FullStr(EMD::EMDC3) << std::endl;
    
    // 测试EME (简单枚举类)
    std::cout << "\n== EME 枚举测试 (简单枚举类) ==" << std::endl;
    std::cout << "起始值 EME1: " << EMEString::FullStr(EME::EME1) << std::endl;
    std::cout << "自动检测 EME2: " << EMEString::FullStr(EME::EME2) << std::endl;
    std::cout << "自动检测 EME3: " << EMEString::FullStr(EME::EME3) << std::endl;
    
    // 测试EMF (普通枚举)
    std::cout << "\n== EMF 枚举测试 (普通枚举) ==" << std::endl;
    std::cout << "起始值 EMFA1: " << EMFString::FullStr(EMF::EMFA1) << std::endl;
    std::cout << "自动检测 EMFA2: " << EMFString::FullStr(EMF::EMFA2) << std::endl;
    std::cout << "自动检测 EMFA3: " << EMFString::FullStr(EMF::EMFA3) << std::endl;
    
    std::cout << "起始值 EMFB1: " << EMFString::FullStr(EMF::EMFB1) << std::endl;
    std::cout << "自动检测 EMFB2: " << EMFString::FullStr(EMF::EMFB2) << std::endl;
    std::cout << "自动检测 EMFB3: " << EMFString::FullStr(EMF::EMFB3) << std::endl;
    
    // 测试短名称
    std::cout << "\n== 短名称测试 ==" << std::endl;
    std::cout << "EMD::EMDA1 短名称: " << EMDString::ShortStr(EMD::EMDA1) << std::endl;
    std::cout << "EMD::EMDA1 长名称: " << EMDString::FullStr(EMD::EMDA1) << std::endl;
    std::cout << "EME::EME2 短名称: " << EMEString::ShortStr(EME::EME2) << std::endl;
    std::cout << "EME::EME2 长名称: " << EMEString::FullStr(EME::EME2) << std::endl;
    std::cout << "EMF::EMFB3 短名称: " << EMFString::ShortStr(EMF::EMFB3) << std::endl;
    std::cout << "EMF::EMFB3 长名称: " << EMFString::FullStr(EMF::EMFB3) << std::endl;
    
    // 测试不存在的枚举值
    std::cout << "\n== 测试不存在的枚举值 ==" << std::endl;
    EMD invalid_enum_d = static_cast<EMD>(19);
    std::cout << "EMD(19) 是否存在: " << (EMDString::Contains(invalid_enum_d) ? "是" : "否") << std::endl;
    std::cout << "EMD(19) 名称: " << EMDString::FullStr(invalid_enum_d) << std::endl;
    
    EME invalid_enum_e = static_cast<EME>(255);
    std::cout << "EME(255) 是否存在: " << (EMEString::Contains(invalid_enum_e) ? "是" : "否") << std::endl;
    std::cout << "EME(255) 名称: " << EMEString::FullStr(invalid_enum_e) << std::endl;

    std::cout << "\n===== 测试完成 =====" << std::endl;
}

} // namespace test_enum_auto 