
#include "../typelist_type.h"
#include "remove.h"

namespace Arrow
{

// 移除尾部类型
template <typename typeList>
struct PopBack;

template <typename... Args>
struct PopBack<TypeList<Args...>>
{
protected:
    using inputType = TypeList<Args...>;
    using removeResult = Remove<inputType::length - 1, inputType>;

public:
    using Tail = typename removeResult::remove_type;
    using type = typename removeResult::type;
};

template <>
struct PopBack<TypeList<>>
{
    typedef TypeList<> Tail;
    typedef TypeList<> type;
};

template <typename typeList>
using PopBack_t = typename PopBack<typeList>::type;

}