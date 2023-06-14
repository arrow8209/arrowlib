
#include "../typelist_type.h"
#include "remove.h"

namespace Arrow
{

namespace tlist
{

// 移除尾部类型
template <typename TList>
struct pop_back;

template <typename... Args>
struct pop_back<TypeList<Args...>>
{
protected:
    typedef TypeList<Args...> TList;
    typedef remove_at<TList::length - 1, TList> remove_tmp;

public:
    typedef typename remove_tmp::remove_type Tail;
    typedef typename remove_tmp::type type;
};

template <>
struct pop_back<TypeList<>>
{
    typedef TypeList<> Tail;
    typedef TypeList<> type;
};

}
}