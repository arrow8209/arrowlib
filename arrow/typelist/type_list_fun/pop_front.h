
#include "../typelist_type.h"

namespace Arrow
{

// 移除头部类型
template <typename typeList>
struct PopFront;

template <typename T, typename... Args>
struct PopFront<TypeList<T, Args...>>
{
    using Head = T;
    using type = TypeList<Args...> ;
};

template <>
struct PopFront<TypeList<>>
{
    using Head = TypeList<>;
    using type = TypeList<>;
};

template <typename typeList>
using PopFront_t = typename PopFront<typeList>::type;

}