
#include "../typelist_type.h"

namespace Arrow
{

namespace tlist
{
// 移除头部类型
template <typename TList>
struct pop_front;

template <typename T, typename... Args>
struct pop_front<TypeList<T, Args...>>
{
    typedef T Head;
    typedef TypeList<Args...> type;
};

template <>
struct pop_front<TypeList<>>
{
    typedef TypeList<> Head;
    typedef TypeList<> type;
};
}
}