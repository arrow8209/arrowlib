/*
 * @FilePath: /arrowlib/arrow/pattern/factory/factory.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-07-07 13:45:37
 * @Description: 运行期抽象工厂，在运行期创建工厂和对应的CreateObj
 */
#pragma once
#include <map>

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

// 抽象工厂
template<typename TObjKeyType, typename TObjBase>
class Factory
{

    typedef TObjBase* (*FunCreate)(); // 具体创建对象的函数
    typedef std::map<TObjKeyType, FunCreate> Map_KeyToFunCreate;

protected:
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

    template<typename TObj>
    void Register(const TObjKeyType& key)
    {
        m_mapKeyToFunCreate[key] = CreateObj<TObjBase, TObj>::Create;
    }
    // 删除所有具体工厂
    void unregisterAll()
    {
        m_mapKeyToFunCreate.clear();
    }
    // 生产类型为_type的产品
    // 失败返回0
    TObjBase* Create(const TObjKeyType& key)
    {
        typename Map_KeyToFunCreate::iterator it = m_mapKeyToFunCreate.find(key);
        if (it != m_mapKeyToFunCreate.end() && it->second != NULL)
        {
            return it->second();
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