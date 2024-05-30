
#pragma once
#include "../typelist_type.h"
#include "push_front.h"

namespace Arrow
{

// 分割typelist遵循左开右闭原则
template <int count, typename typeList>
struct Splite;

template <typename T, typename... Args>
struct Splite<1, TypeList<T, Args...>>
{
    using Head = TypeList<T>;
    using Tail = TypeList<Args...>;
};

template <int count, typename T, typename... Args>
struct Splite<count, TypeList<T, Args...>>
{
protected:
    using spliteNext = Splite<count - 1, TypeList<Args...>>;

    static_assert(count > 0, "count 必须 >0");
    static_assert(count <= sizeof...(Args), "count 必须小于列表的总长度 -1 ");

public:
    using Head = PushFront_t<T, typename spliteNext::Head>;
    using Tail = typename spliteNext::Tail;
};

template <int count, typename typeList>
using SpliteHead = typename Splite<count, typeList>::Head;

template <int count, typename typeList>
using SpliteTail = typename Splite<count, typeList>::Tail;

}