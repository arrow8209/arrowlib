#pragma once
#include <typeinfo>

namespace Arrow
{

namespace Pattern
{

template<typename TObjBase, typename TObj>
class CreateObj
{
public:
    // 生产一个具体产品
    static TObjBase* Create()
    {
        return new TObj();
    }

};

} // namespace ArrwoPattern
}