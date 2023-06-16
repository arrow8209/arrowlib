#include <iostream>
#include <string_view>

// 定义 TypeList 结构体
template <typename... Types>
struct TypeList {};

// 定义转换函数
template <typename StringView>
struct ConvertStringViewToList;

// 基本情况：当 std::string_view 为空时返回空的 TypeList
template <>
struct ConvertStringViewToList<std::string_view> {
    using Result = TypeList<>;
};

// 递归情况：将 std::string_view 拆分为头部和尾部，然后递归处理尾部
template <char Delimiter, char... Chars>
struct ConvertStringViewToList<std::integer_sequence<char, Delimiter, Chars...>> {
    using Head = std::integer_sequence<char, Delimiter>;
    using Tail = typename ConvertStringViewToList<std::integer_sequence<char, Chars...>>::Result;
    using Result = typename std::conditional<sizeof...(Chars) == 0, TypeList<Head>, TypeList<Head, Tail>>::type;
};

// 辅助函数：将 std::string_view 转换为 TypeList
template <typename StringView>
constexpr auto convertStringViewToList(StringView sv) {
    return typename ConvertStringViewToList<std::make_integer_sequence<char, sv.size()>>::Result{};
}

int main() {
    std::string_view sv = "1,2,3,4,5";
    using ResultType = decltype(convertStringViewToList(sv));
    std::cout << "Result Type: ";
    // 输出转换后的 TypeList
    std::cout << typeid(ResultType).name() << std::endl;

    return 0;
}
