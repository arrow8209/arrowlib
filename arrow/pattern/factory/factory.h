/*
 * @FilePath: /arrowlib/arrow/pattern/factory/factory.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-07-07 13:45:37
 * @Description: 运行期抽象工厂，在运行期创建工厂和对应的CreateObj
 */
#pragma once
#include <map>
#include <functional>
#include <tuple>
#include "create_obj.h"

namespace Arrow
{

namespace Pattern
{

// 抽象工厂
template<typename KeyType, typename ObjBase, typename ...CreateParmList>
class Factory
{
    using FunCreate = std::function<ObjBase*(CreateParmList...)>;
    using Map_KeyToFunCreate = std::map<KeyType, FunCreate>;

public:
    Factory() = default;
    Factory(const Factory&) = delete;
    Factory& operator=(const Factory&) = delete;

public:
    virtual ~Factory() 
    {
        m_mapKeyToFunCreate.clear();
    }

    // 对外接口
public:

    /**
     * @description:  注册类型，使用默认的new方式创建
     * @param {KeyType&} key
     * @return {*}
     */
    template<typename TObj>
    bool Register(const KeyType& key)
    {
        if(m_mapKeyToFunCreate.find(key) != m_mapKeyToFunCreate.end())
        {
            return false;
        }
        m_mapKeyToFunCreate[key] = &Arrow::Other::CreateUseNew<TObj>::Create;
        return true;
    }

    /**
     * @description: 由用户提供创建函数地址
     * @param {KeyType&} key
     * @param {FunCreate} funCreate
     * @return {*}
     */
    template <typename TObj>
    bool Register(const KeyType& key, FunCreate funCreate)
    {
        if (m_mapKeyToFunCreate.find(key) != m_mapKeyToFunCreate.end())
        {
            return false;
        }
        m_mapKeyToFunCreate[key] = funCreate;
        return true;
    }

    // 删除所有具体工厂
    void UnregisterAll()
    {
        m_mapKeyToFunCreate.clear();
    }

    // 生产类型为_type的产品
    // 失败返回nullptr

    ObjBase* Create(const KeyType& key, CreateParmList... param)
    {
        typename Map_KeyToFunCreate::iterator it = m_mapKeyToFunCreate.find(key);
        if (it != m_mapKeyToFunCreate.end() && it->second != NULL)
        {
            return it->second(param...);
        }
        return nullptr;
    }
private:
    Map_KeyToFunCreate m_mapKeyToFunCreate; // 保存所有注册过的具体工厂
};

//Demo
// class FactoryDeocder : public Arrow::Pattern::Factory<std::string, IBase>
// {
// public:
//     FactoryDeocder()
//     {
//         Register<CannDecoder>("Name1");
//     }
// };
//
// typedef Arrow::Pattern::Singleton<FactoryDeocder>  App_FactoryDecoder;
//IBase* pObj = App_FactoryDecoder::Instance()->New("Name1");
}
}