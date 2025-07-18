/**
 * @file test_enum_to_string_alt.h
 * @brief 测试替代的枚举到字符串转换实现
 */

#pragma once

#include <iostream>
#include "enum_to_string_alt.h"

// 重用相同的枚举定义
#include "test_enum_to_string.h"

// 辅助宏，在当前命名空间中使用枚举
#define ENUM_VALUE(name) name,

namespace test_enum_alt {

// 使用X宏方式定义一个新的枚举（这里仅作演示）
#define MY_ENUM_VALUES(X) \
    X(Value1) \
    X(Value2) \
    X(Value3)

enum class MyEnum { MY_ENUM_VALUES(ENUM_VALUE) };

} // namespace test_enum_alt

// 为枚举定义转换映射（必须在全局命名空间）

// 为EMA定义转换映射
DEFINE_ENUM_TO_STRING(EMA,
    {{EMA::EMA1, "EMA::EMA1", "EMA1"}},
    {{EMA::EMA2, "EMA::EMA2", "EMA2"}},
    {{EMA::EMA3, "EMA::EMA3", "EMA3"}}
);

// 为EMB定义转换映射
DEFINE_ENUM_TO_STRING(EMB,
    {{EMB::EMB1, "EMB::EMB1", "EMB1"}},
    {{EMB::EMB2, "EMB::EMB2", "EMB2"}},
    {{EMB::EMB3, "EMB::EMB3", "EMB3"}}
);

// 为EMC定义转换映射
DEFINE_ENUM_TO_STRING(EMC,
    {{EMC::EMCA1, "EMC::EMCA1", "EMCA1"}},
    {{EMC::EMCA2, "EMC::EMCA2", "EMCA2"}},
    {{EMC::EMCA3, "EMC::EMCA3", "EMCA3"}},
    {{EMC::EMCB1, "EMC::EMCB1", "EMCB1"}},
    {{EMC::EMCB2, "EMC::EMCB2", "EMCB2"}},
    {{EMC::EMCB3, "EMC::EMCB3", "EMCB3"}},
    {{EMC::EMCC1, "EMC::EMCC1", "EMCC1"}},
    {{EMC::EMCC2, "EMC::EMCC2", "EMCC2"}},
    {{EMC::EMCC3, "EMC::EMCC3", "EMCC3"}}
);

// 为自定义枚举创建映射
DEFINE_ENUM_TO_STRING_SIMPLE(test_enum_alt::MyEnum,
    {{test_enum_alt::MyEnum::Value1, "test_enum_alt::MyEnum::Value1", "Value1"}},
    {{test_enum_alt::MyEnum::Value2, "test_enum_alt::MyEnum::Value2", "Value2"}},
    {{test_enum_alt::MyEnum::Value3, "test_enum_alt::MyEnum::Value3", "Value3"}}
);

namespace test_enum_alt {

// 运行测试函数
inline void run_test() {
    std::cout << "\n===== 替代实现：枚举到字符串转换测试 =====" << std::endl;
    
    // 使用类型别名简化代码
    using EMAString = enum_utils_alt::enum_to_string<EMA>;
    using EMBString = enum_utils_alt::enum_to_string<EMB>;
    using EMCString = enum_utils_alt::enum_to_string<EMC>;
    using MyEnumString = enum_utils_alt::enum_to_string<MyEnum>;
    
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
    
    // 测试自定义枚举
    std::cout << "\n== 自定义枚举测试 ==" << std::endl;
    std::cout << "完整名称: " << MyEnumString::Str(MyEnum::Value1) << std::endl;
    std::cout << "短名称: " << MyEnumString::ShortStr(MyEnum::Value1) << std::endl;
    std::cout << "完整名称: " << MyEnumString::Str(MyEnum::Value2) << std::endl;
    std::cout << "短名称: " << MyEnumString::ShortStr(MyEnum::Value2) << std::endl;
    
    std::cout << "\n===== 测试完成 =====" << std::endl;
}

} // namespace test_enum_alt 