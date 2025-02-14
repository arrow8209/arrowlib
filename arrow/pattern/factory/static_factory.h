/*
 * @FilePath: /arrowlib/arrow/pattern/factory/static_factory.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-06-30 13:35:33
 * @Description: 编译期抽象工厂 在编译器创建好工厂和 CreatorObj
 */
#pragma once
#include <map>

namespace Arrow
{

namespace Pattern
{

template <int _key, typename _TObj>
class Static_CreateObj
{
public:
    typedef _TObj TObj;
    static int Key;
    // 生产一个具体产品
    static TObj* Create(int t1)
    {
        if (t1 == Key)
        {
            return new TObj();
        }
        else
        {
            return nullptr;
        }
    }
};
template <int _key, typename _TObj>
int Static_CreateObj<_key, _TObj>::Key = _key;

// 抽象工厂
template <typename TObjBase, typename TCreateorObj, typename... _Args>
class Staitic_Factory
{

protected:
    Staitic_Factory() = default;
    Staitic_Factory(const Staitic_Factory&) = delete;
    Staitic_Factory& operator=(const Staitic_Factory&) = delete;
    virtual ~Staitic_Factory() = default;

public:
    static TObjBase* Create(const int& t)
    {
        if (t == TCreateorObj::Key)
        {
            return dynamic_cast<TObjBase*>(TCreateorObj::Create(t));
        }

        return Factory<TObjBase, _Args...>::Create(t);
    }
};

template <typename TObjBase, typename TCreateorObj>
class Staitic_Factory<TObjBase, TCreateorObj>
{

protected:
    Staitic_Factory() = default;
    Staitic_Factory(const Staitic_Factory&) = delete;
    Staitic_Factory& operator=(const Staitic_Factory&) = delete;
    virtual ~Staitic_Factory() = default;

public:
    static TObjBase* Create(const int& t)
    {
        if (t == TCreateorObj::Key)
        {
            return dynamic_cast<TObjBase*>(TCreateorObj::Create(t));
        }

        return nullptr;
    }
};

//Demo
// tyepdef typedef Staitic_Factory<Base,
//                 CreateObj<1, A>,
//                 CreateObj<2, B>,
//                 CreateObj<3, C>,
//                 CreateObj<4, D>>
//     App_Factory;

} 
}