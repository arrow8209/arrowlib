/**
 * @file enum_to_string_alt.h
 * @brief 枚举到字符串转换的替代实现（不使用__PRETTY_FUNCTION__）
 * 
 * 这个实现展示了一种不依赖__PRETTY_FUNCTION__的枚举到字符串转换方法。
 * 它使用静态映射表，更简单且跨平台，但需要手动维护。
 */

#pragma once

#include <string>
#include <unordered_map>
#include <type_traits>

// 辅助宏，用于枚举值定义
#define ENUM_VALUE(name) name,

namespace enum_utils_alt {

/**
 * @brief 枚举到字符串转换基类模板
 * 
 * 这个模板需要为每个枚举类型进行特化。
 */
template <typename EnumT>
class enum_to_string {
public:
    static std::string Str(EnumT value);
    static std::string ShortStr(EnumT value);
    static bool Contains(EnumT value);
};

// 一个辅助宏，生成带有枚举类型前缀的字符串常量
#define FULL_ENUM_NAME(type, name) #type "::" #name

/**
 * @brief 宏：定义枚举到字符串的转换器（带完整类型名）
 * 
 * 使用方式:
 * DEFINE_ENUM_TO_STRING(EnumType,
 *     {EnumType::Value1, FULL_ENUM_NAME(EnumType, Value1), "Value1"},
 *     {EnumType::Value2, FULL_ENUM_NAME(EnumType, Value2), "Value2"},
 *     ...
 * )
 */
#define DEFINE_ENUM_TO_STRING(EnumType, ...) \
template <> \
class enum_utils_alt::enum_to_string<EnumType> { \
private: \
    struct mapping_entry { \
        EnumType value; \
        const char* full_name; \
        const char* short_name; \
    }; \
    static const std::unordered_map<EnumType, mapping_entry> enum_map; \
    static const mapping_entry* find_entry(EnumType value) { \
        auto it = enum_map.find(value); \
        return (it != enum_map.end()) ? &it->second : nullptr; \
    } \
public: \
    static std::string Str(EnumType value, const char* default_str = "(unknown)") { \
        const mapping_entry* entry = find_entry(value); \
        return entry ? entry->full_name : default_str; \
    } \
    static std::string ShortStr(EnumType value, const char* default_str = "(unknown)") { \
        const mapping_entry* entry = find_entry(value); \
        return entry ? entry->short_name : default_str; \
    } \
    static bool Contains(EnumType value) { \
        return find_entry(value) != nullptr; \
    } \
}; \
\
const std::unordered_map<EnumType, enum_utils_alt::enum_to_string<EnumType>::mapping_entry> \
enum_utils_alt::enum_to_string<EnumType>::enum_map = { \
    __VA_ARGS__ \
}

/**
 * @brief 简化版：定义枚举到字符串的转换器
 */
#define DEFINE_ENUM_TO_STRING_SIMPLE(EnumType, ...) DEFINE_ENUM_TO_STRING(EnumType, __VA_ARGS__)

} // namespace enum_utils_alt 