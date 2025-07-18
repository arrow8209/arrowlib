/**
 * @file enum_to_string_auto.h
 * @brief 自动识别枚举项的枚举到字符串转换库
 *
 * 本库提供一个简单、易用的接口将枚举值转换为其对应的字符串表示。
 * 支持普通枚举和枚举类(enum class)，支持不连续的枚举值，并且能够根据起始枚举值自动识别同一范围内的所有枚举值。
 */

 #pragma once

 // 确保使用C++17标准
 #include <cstddef>
 #include <limits>
 #if __cplusplus < 201703L
 #error 此库需要C++17或更高版本的支持
 #endif
 
 #include <array>
 #include <iostream>
 #include <string>
 #include <string_view>
 #include <tuple>
 #include <type_traits>
 #include <utility>
 
 namespace enum_utils
 {
 
 // 调试函数，打印__PRETTY_FUNCTION__的内容
 template <typename EnumType, EnumType E> void debug_print_pretty_function()
 {
     // GCC Output:
     // DEBUG [void enum_utils::debug_print_pretty_function() [with EnumType = EMD; EnumType E = EMDA1]]
     // DEBUG [void enum_utils::debug_print_pretty_function() [with EnumType = EMD; EnumType E = (EMD)19]]
     // DEBUG [void enum_utils::debug_print_pretty_function() [with EnumType = EME; EnumType E = EME::EME1]]
 
     // Clang Output:
     // DEBUG [void enum_utils::debug_print_pretty_function() [EnumType = EMD, E = EMDA1]]
     // DEBUG [void enum_utils::debug_print_pretty_function() [EnumType = EME, E = EME::EME1]]
     // DEBUG [void enum_utils::debug_print_pretty_function() [EnumType = EMD, E = (EMD)19]]
     
 
     std::cout << "DEBUG [" << __PRETTY_FUNCTION__ << "]" << std::endl;
 }
 
 // 计算字符串长度
 constexpr std::size_t str_length(const char* str)
 {
     std::size_t len = 0;
     while (str != nullptr && str[len] != '\0')
         ++len;
     return len;
 }
 
 // 自定义字符串视图类，用于编译期字符串处理
 class string_view
 {
 private:
     const char* data_;
     std::size_t size_;
 
 public:
     static constexpr std::size_t npos = static_cast<std::size_t>(-1);
 
     constexpr string_view() : data_(nullptr), size_(0) {}
     constexpr string_view(const char* str) : data_(str), size_(str_length(str)) {}
     constexpr string_view(const char* str, std::size_t len) : data_(str), size_(len) {}
     constexpr string_view(const ::std::string_view& sv) : data_(sv.data()), size_(sv.size()) {}
 
     constexpr const char* data() const { return data_; }
     constexpr std::size_t size() const { return size_; }
     constexpr bool empty() const { return size_ == 0; }
 
     constexpr char operator[](std::size_t pos) const { return pos < size_ ? data_[pos] : '\0'; }
 
     constexpr string_view substr(std::size_t pos, std::size_t count = npos) const
     {
         if (pos >= size_)
             return string_view();
         std::size_t rcount = (count == npos || pos + count > size_) ? (size_ - pos) : count;
         return string_view(data_ + pos, rcount);
     }
 
     constexpr std::size_t find(char ch, std::size_t pos = 0) const
     {
         for (std::size_t i = pos; i < size_; ++i)
         {
             if (data_[i] == ch)
                 return i;
         }
         return npos;
     }
 
     constexpr std::size_t rfind(char ch) const
     {
         if (size_ == 0)
             return npos;
         for (std::size_t i = size_ - 1; i != static_cast<std::size_t>(-1); --i)
         {
             if (data_[i] == ch)
                 return i;
         }
         return npos;
     }
 
     constexpr std::size_t find(const string_view& str, std::size_t pos = 0) const
     {
         if (pos >= size_ || str.size_ > size_ - pos || str.empty())
         {
             return npos;
         }
 
         for (std::size_t i = pos; i <= size_ - str.size_; ++i)
         {
             bool found = true;
             for (std::size_t j = 0; j < str.size_; ++j)
             {
                 if (data_[i + j] != str.data_[j])
                 {
                     found = false;
                     break;
                 }
             }
             if (found)
                 return i;
         }
 
         return npos;
     }
 
     constexpr bool operator==(const string_view& rhs) const
     {
         if (size_ != rhs.size_)
             return false;
         for (std::size_t i = 0; i < size_; ++i)
         {
             if (data_[i] != rhs.data_[i])
                 return false;
         }
         return true;
     }
 
     // 转换为std::string
     std::string to_string() const { return std::string(data_, size_); }
 };
 
 /**
  * @brief 为不同的编译器实现枚举名称解析
  */
 template <typename EnumType, EnumType E> constexpr string_view get_enum_name_impl()
 {
     // 获取函数签名
     constexpr string_view signature = __PRETTY_FUNCTION__;
 
 #if defined(__clang__)
     // Clang: "string_view enum_utils::get_enum_name_impl() [EnumType = EMA, E = EMA::EMA1]"
     constexpr auto open_bracket = signature.find('[');
     if (open_bracket == string_view::npos)
         return string_view("(unknown)");
 
     constexpr auto close_bracket = signature.find(']', open_bracket);
     if (close_bracket == string_view::npos)
         return string_view("(unknown)");
 
     // 获取括号内的内容，例如 "EnumType = EMA, E = EMA::EMA1"
     constexpr auto inside_brackets = signature.substr(open_bracket + 1, close_bracket - open_bracket - 1);
 
     // 找到 "E = " 后面的部分
     constexpr auto e_equals_pos = inside_brackets.find("E = ");
     if (e_equals_pos == string_view::npos)
         return string_view("(unknown)");
 
     constexpr auto enum_value_start = e_equals_pos + 4; // 跳过 "E = "
     constexpr auto enum_value_text = inside_brackets.substr(enum_value_start);
 
     return enum_value_text; // 返回完整的枚举名称，包括作用域
 
 #elif defined(__GNUC__) || defined(__GNUG__)
     // GCC: "constexpr enum_utils::string_view enum_utils::get_enum_name_impl() [with EnumType = EMA; E = EMA::EMA1]"
     constexpr auto with_pos = signature.find("with");
     if (with_pos == string_view::npos)
         return string_view("(unknown)");
 
     constexpr auto e_equals_pos = signature.find("E = ", with_pos);
     if (e_equals_pos == string_view::npos)
         return string_view("(unknown)");
 
     constexpr auto enum_value_start = e_equals_pos + 4; // 跳过 "E = "
     constexpr auto semicolon_pos = signature.find(']', enum_value_start);
 
     constexpr auto enum_value_end = (semicolon_pos != string_view::npos) ? semicolon_pos : signature.size();
 
     constexpr auto enum_value_text = signature.substr(enum_value_start, enum_value_end - enum_value_start);
 
     return enum_value_text; // 返回完整的枚举名称，包括作用域
 
 #elif defined(_MSC_VER)
     // MSVC处理
     return string_view("(unsupported compiler)");
 #else
     return string_view("(unsupported compiler)");
 #endif
 }
 
 /**
  * @brief 获取枚举类型的名称前缀
  */
 template <typename EnumType> constexpr string_view get_enum_type_prefix()
 {
 #if defined(__clang__)
     // Clang实现
     constexpr string_view signature = __PRETTY_FUNCTION__;
 
     constexpr auto type_start = signature.find("EnumType = ");
     if (type_start == string_view::npos)
     {
         return string_view("(unknown)");
     }
 
     constexpr auto name_start = type_start + 11; // 跳过"EnumType = "
     constexpr auto comma_pos = signature.find(',', name_start);
 
     if (comma_pos != string_view::npos)
     {
         return signature.substr(name_start, comma_pos - name_start);
     }
 
     return signature.substr(name_start);
 #elif defined(__GNUC__) || defined(__GNUG__)
     // GCC实现
     constexpr string_view signature = __PRETTY_FUNCTION__;
 
     // 查找"EnumType ="
     constexpr auto type_start = signature.find("EnumType = ");
     if (type_start == string_view::npos)
     {
         // 如果找不到，尝试查找"with EnumType ="
         constexpr auto with_type_start = signature.find("with EnumType = ");
         if (with_type_start == string_view::npos)
         {
             return string_view("(unknown)");
         }
 
         constexpr auto name_start = with_type_start + 15; // 跳过"with EnumType = "
 
         // 查找分号或方括号，取最先出现的
         constexpr auto semicolon_pos = signature.find(';', name_start);
         constexpr auto bracket_pos = signature.find(']', name_start);
         constexpr auto end_pos =
             (semicolon_pos != string_view::npos && (bracket_pos == string_view::npos || semicolon_pos < bracket_pos))
                 ? semicolon_pos
                 : bracket_pos;
 
         if (end_pos != string_view::npos)
         {
             return signature.substr(name_start, end_pos - name_start);
         }
 
         return signature.substr(name_start);
     }
 
     constexpr auto name_start = type_start + 11; // 跳过"EnumType = "
 
     // 查找逗号或方括号，取最先出现的
     constexpr auto comma_pos = signature.find(',', name_start);
     constexpr auto bracket_pos = signature.find(']', name_start);
     constexpr auto end_pos = (comma_pos != string_view::npos && (bracket_pos == string_view::npos || comma_pos < bracket_pos))
                                  ? comma_pos
                                  : bracket_pos;
 
     if (end_pos != string_view::npos)
     {
         return signature.substr(name_start, end_pos - name_start);
     }
 
     return signature.substr(name_start);
 #else
     // 其他编译器
     return string_view(typeid(EnumType).name());
 #endif
 }
 
 /**
  * @brief 获取枚举名称的公共接口（仅枚举值名称，不包含枚举类型名）
  */
 template <typename EnumType, EnumType E> constexpr string_view get_enum_short_name()
 {
     constexpr auto full_name = get_enum_name_impl<EnumType, E>();
     constexpr auto last_colon_pos = full_name.rfind(':');
 
     if (last_colon_pos == string_view::npos || last_colon_pos == 0)
     {
         return full_name;
     }
 
     return full_name.substr(last_colon_pos + 1);
 }
 
 /**
  * @brief 获取枚举完整名称的公共接口（包含枚举类型名）
  */
 template <typename EnumType, EnumType E> constexpr string_view get_enum_name()
 {
     constexpr auto name = get_enum_name_impl<EnumType, E>();
 
     // 检查名称中是否已经包含作用域操作符::
     constexpr auto has_scope = name.find(':') != string_view::npos;
 
     // 如果已经包含作用域，则直接返回
     if (has_scope)
     {
         return name;
     }
 
     // 对于普通枚举，需要手动添加枚举类型名称作为前缀
     // 注意：这里无法在constexpr上下文中拼接字符串，所以我们只能返回短名称
     // 在实际使用时，我们会在Str方法中处理这种情况
     return name;
 }
 
 /**
  * @brief 判断是否为枚举类（而非普通枚举）
  */
 template <typename EnumType> constexpr bool is_enum_class()
 {
     // 枚举类不能隐式转换为整数类型
     return !std::is_convertible<EnumType, int>::value;
 }
 
 template <typename EnumType, EnumType StartValue, std::size_t Offset> 
 struct is_valid_enum_value
 {
     template <typename U, EnumType enumValue = static_cast<EnumType>(static_cast<std::underlying_type_t<EnumType>>(StartValue) + Offset)>
     static constexpr bool test(U *) 
     {
         // 尝试创建一个整数值
         constexpr auto int_value = static_cast<std::underlying_type_t<EnumType>>(StartValue) + Offset;
 
         // 尝试将整数值转换回枚举类型
         constexpr auto enum_value = static_cast<EnumType>(int_value);
 
         // 尝试获取枚举值的名称
         constexpr auto name = get_enum_name_impl<EnumType, enum_value>();
 
         // 如果名称包含invalid、unknown或unknown_enum_value，或者包含括号(表示显式转换)，则认为不是有效的枚举值
         constexpr auto is_invalid = name.find("invalid") != string_view::npos || name.find("unknown") != string_view::npos ||
                                     name.find("unknown_enum_value") != string_view::npos ||
                                     name.find('(') != string_view::npos || // 检查是否包含左括号
                                     name.find(')') != string_view::npos;   // 检查是否包含右括号
 
         return !is_invalid;
     }
     static constexpr bool test(...) { return false; }
 
     static constexpr bool value = test(static_cast<EnumType *>(0));
 };
 
 // 枚举范围探测助手类
 template <typename EnumType, EnumType StartValue, std::size_t MaxOffset, std::size_t CurrentOffset = 0, typename Indices = std::index_sequence<>>
 struct enum_range_detector;
 
 // 当达到最大偏移或找不到有效枚举值时终止递归
 template <typename EnumType, EnumType StartValue, std::size_t MaxOffset, std::size_t CurrentOffset, std::size_t... Indices>
 struct enum_range_detector<EnumType, StartValue, MaxOffset, CurrentOffset, std::index_sequence<Indices...>>
 {
 private:
     // 检查当前偏移量的枚举值是否有效
     static constexpr bool is_current_valid = is_valid_enum_value<EnumType, StartValue, CurrentOffset>::value;
     static constexpr bool is_next_detect = is_current_valid && CurrentOffset < MaxOffset;
     using indices =
         std::conditional_t<is_current_valid, std::index_sequence<Indices..., CurrentOffset>, std::index_sequence<Indices...>>;
 
     // 决定是否继续递归
     using next_detector = std::conditional_t<is_next_detect,
                                              enum_range_detector<EnumType, StartValue, MaxOffset, CurrentOffset + 1, indices>,
                                              enum_range_detector<EnumType, StartValue, MaxOffset, MaxOffset, indices>>;
 public:
     // 收集所有有效的偏移量
     // using type = typename next_detector::type;
     using type = typename next_detector::type;
 };
 
 // 终止条件
 template <typename EnumType, EnumType StartValue, std::size_t MaxOffset, std::size_t... Indices>
 struct enum_range_detector<EnumType, StartValue, MaxOffset, MaxOffset, std::index_sequence<Indices...>>
 {
     using type = std::index_sequence<Indices...>;
 };
 
 // 获取枚举类型的最大偏移量
 template <typename EnumType, EnumType StartValue>
 static constexpr std::size_t max_offset = std::numeric_limits<std::underlying_type_t<EnumType>>::max() - static_cast<std::underlying_type_t<EnumType>>(StartValue);
 
 // 获取枚举范围内的所有有效值
 template <typename EnumType, EnumType StartValue>
 using enum_range_detector_t = typename enum_range_detector<EnumType, StartValue, max_offset<EnumType, StartValue>>::type;
 
 // 将偏移量序列转换为枚举值序列
 template <typename EnumType, EnumType StartValue, typename index_sequence_offsets>
 struct make_enum_values_from_offsets;
 
 template <typename EnumType, EnumType StartValue, std::size_t... Offsets>
 struct make_enum_values_from_offsets<EnumType, StartValue, std::index_sequence<Offsets...>>
 {
     static constexpr auto base_value = static_cast<std::underlying_type_t<EnumType>>(StartValue);
     // static constexpr auto value = std::array<EnumType, sizeof...(Offsets)>{static_cast<EnumType>(base_value + Offsets)...};
     using type = std::index_sequence<base_value + Offsets...>;
 };
 
 // 获取EnumType有效序列
 template <typename EnumType, EnumType StartValue>
 struct make_enum_values
 {
     using index_sequence_offsets = enum_range_detector_t<EnumType, StartValue>;
     // static constexpr auto value = make_enum_values_from_offsets<EnumType, StartValue, index_sequence_offsets>::value;
     using type = typename make_enum_values_from_offsets<EnumType, StartValue, index_sequence_offsets>::type;
 };
 
 // 工具：拼接两个 index_sequence
 template<typename S1, typename S2>
 struct index_sequence_concat;
 
 template <std::size_t... I1, std::size_t... I2>
 struct index_sequence_concat<std::index_sequence<I1...>, std::index_sequence<I2...>>
 {
     using type = std::index_sequence<I1..., I2...>;
 };
 
 template <typename EnumType, EnumType... StartValues>
 struct enum_auto_detector_impl;
 
 template <typename EnumType, EnumType StartValue, EnumType... StartValues>
 struct enum_auto_detector_impl<EnumType, StartValue, StartValues...>
 {
     using type = typename index_sequence_concat<typename make_enum_values<EnumType, StartValue>::type, typename enum_auto_detector_impl<EnumType, StartValues...>::type>::type;
 };
 
 template <typename EnumType, EnumType StartValue>
 struct enum_auto_detector_impl<EnumType, StartValue>
 {
     using type = typename make_enum_values<EnumType, StartValue>::type;
 };
 
 /**
  * @brief 枚举到字符串转换类，通过提供起始值自动检测枚举值
  * @tparam EnumT 枚举类型
  * @tparam StartValues 枚举范围的起始值
  */
 template <typename EnumT, EnumT... StartValues> class enum_to_string
 {
     // 创建一个包含所有枚举项的映射数组
     struct mapping_pair
     {
         EnumT enum_value;
         string_view full_name;
         string_view short_name;
         string_view type_prefix;
         bool is_scoped;
 
         constexpr mapping_pair(EnumT value, string_view full, string_view short_, string_view prefix, bool scoped)
             : enum_value(value), full_name(full), short_name(short_), type_prefix(prefix), is_scoped(scoped)
         {
         }
     };
 
     // 为每个检测到的枚举值创建映射
     template <std::size_t... Is> static constexpr auto create_mappings_impl(std::index_sequence<Is...>)
     {
         return std::array<mapping_pair, sizeof...(Is)>{mapping_pair{static_cast<EnumT>(Is),
                                                                     get_enum_name<EnumT, static_cast<EnumT>(Is)>(),
                                                                     get_enum_short_name<EnumT, static_cast<EnumT>(Is)>(),
                                                                     get_enum_type_prefix<EnumT>(),
                                                                     is_enum_class<EnumT>()}...};
     }
     
     using enum_auto_detector_impl_t = typename enum_auto_detector_impl<EnumT, StartValues...>::type;
 
     static constexpr auto mappings = create_mappings_impl(enum_auto_detector_impl_t{});
 
     // 查找枚举值对应的索引
     static constexpr std::size_t find_enum_index(EnumT value)
     {
         for (std::size_t i = 0; i < mappings.size(); ++i)
         {
             if (mappings[i].enum_value == value)
                 return i;
         }
         return mappings.size(); // 未找到
     }
 
 public:
     /**
      * @brief 将枚举值转换为完整的字符串（包含枚举类型名）
      * @param value 要转换的枚举值
      * @param default_str 未找到时返回的默认字符串
      * @return 枚举值的字符串表示
      */
     static std::string FullStr(EnumT value, const char* default_str = "(unknown)")
     {
         const std::size_t idx = find_enum_index(value);
         if (idx < mappings.size())
         {
             const auto& mapping = mappings[idx];
             const string_view& name = mapping.full_name;
 
             // 如果是普通枚举且名称中不包含作用域操作符，则添加枚举类型前缀
             if (!mapping.is_scoped && name.find(':') == string_view::npos)
             {
                 std::string result = mapping.type_prefix.to_string();
                 result += "::";
                 result += name.to_string();
                 return result;
             }
 
             return name.to_string();
         }
         return std::string(default_str);
     }
 
     /**
      * @brief 将枚举值转换为短字符串（不包含枚举类型名）
      * @param value 要转换的枚举值
      * @param default_str 未找到时返回的默认字符串
      * @return 枚举值的短字符串表示
      */
     static std::string ShortStr(EnumT value, const char* default_str = "(unknown)")
     {
         const std::size_t idx = find_enum_index(value);
         if (idx < mappings.size())
         {
             const string_view& name = mappings[idx].short_name;
             return name.to_string();
         }
         return std::string(default_str);
     }
 
     /**
      * @brief 判断枚举值是否有对应的字符串表示
      * @param value 要检查的枚举值
      * @return 如果存在对应名称则返回true，否则返回false
      */
     static constexpr bool Contains(EnumT value) { return find_enum_index(value) < mappings.size(); }
 };
 
 } // namespace enum_utils