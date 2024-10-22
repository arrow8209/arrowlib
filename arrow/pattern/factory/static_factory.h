/*
 * @FilePath: /arrowlib/arrow/pattern/factory/static_factory.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-06-30 13:35:33
 * @Description: 编译期抽象工厂 在编译期创建好工厂对象
 */
#pragma once
#include <map>

namespace Arrow
{

namespace Pattern
{

template <typename KeyType, KeyType key, typename Obj>
struct StaticFactoryAssist
{};

template <typename KeyType, typename TObjBase, typename ...Args>
class StaticFactory
{};

// 抽象工厂
template <typename KeyType, typename TObjBase, KeyType keyValue, typename Obj, typename ...Args>
class StaticFactory<KeyType, TObjBase, StaticFactoryAssist<KeyType, keyValue, Obj>, Args...>
{
protected:
    StaticFactory() = default;
    StaticFactory(const StaticFactory&) = delete;
    StaticFactory& operator=(const StaticFactory&) = delete;
    virtual ~StaticFactory() = default;

public:
    static TObjBase* Create(const KeyType& t)
    {
        if (t == keyValue)
        {
            return dynamic_cast<TObjBase*>(new Obj());
        }

        return StaticFactory<KeyType, TObjBase, Args...>::Create(t);
    }

    template<typename ...CreateArgs>
    static TObjBase* Create(const KeyType& t, CreateArgs&&... args)
    {
        if (t == keyValue)
        {
            return dynamic_cast<TObjBase*>(new Obj(args...));
        }

        return StaticFactory<KeyType, TObjBase, Args...>::Create(t, std::forward<CreateArgs>(args)...);
    }
};

template <typename KeyType, typename TObjBase, KeyType keyValue, typename Obj>
class StaticFactory<KeyType, TObjBase, StaticFactoryAssist<KeyType, keyValue, Obj>>
{
protected:
    StaticFactory() = default;
    StaticFactory(const StaticFactory&) = delete;
    StaticFactory& operator=(const StaticFactory&) = delete;
    virtual ~StaticFactory() = default;

public:
    static TObjBase* Create(const KeyType& t)
    {
        if (t == keyValue)
        {
            return dynamic_cast<TObjBase*>(new Obj());
        }
        return nullptr;
    }

    template<typename ...CreateArgs>
    static TObjBase* Create(const KeyType& t, CreateArgs&&... args)
    {
        if (t == keyValue)
        {
            return dynamic_cast<TObjBase*>(new Obj(args...));
        }

        return nullptr;
    }
};

//Demo
// tyepdef typedef StaticFactory<int, Base,
//                 std::integral_constant<int, 1> A,
//                 std::integral_constant<int, 2> B,
//                 std::integral_constant<int, 3> C,
//                 std::integral_constant<int, 4> D>
//     App_Factory;

} 
}