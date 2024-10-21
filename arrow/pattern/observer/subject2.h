/*
 * @FilePath: /arrow/pattern/observer/subject2.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2023-10-31 16:51:28
 * @Description:复杂观察着。消息接受者，通过函数指针接收不同的Subject发送过来的消息
 */
#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <algorithm>
#include <functional>
#include <tuple>
#include <map>
#include "../../other/increment.h"
#include "../../typelist/typelist.h"

namespace Arrow
{

namespace Pattern
{

namespace details
{
template <typename Obj, typename ObjFunPtr>
struct ObserverAssist
{
public:
    using Local = ObserverAssist<Obj, ObjFunPtr>;
    using Increment = Arrow::Other::TIncrement<Local, uint32_t, 1>;
    using VecObjFunPtr = std::vector<ObjFunPtr>;

    static int32_t GetID(ObjFunPtr objFunPtr)
    {
        std::lock_guard<std::mutex> lock(gMutex);
        auto it = std::find(gVecObjFunPtr.begin(), gVecObjFunPtr.end(),  objFunPtr);
        if(it != gVecObjFunPtr.end())
        {
            return std::distance(gVecObjFunPtr.begin(), it);
        }
        gVecObjFunPtr.push_back(objFunPtr);
        return gVecObjFunPtr.size() - 1;
    }
    static std::mutex gMutex;
    static VecObjFunPtr gVecObjFunPtr;
};
template <typename Obj, typename ObjFunPtr>
typename ObserverAssist<Obj, ObjFunPtr>::VecObjFunPtr ObserverAssist<Obj, ObjFunPtr>::gVecObjFunPtr;
template <typename Obj, typename ObjFunPtr>
std::mutex ObserverAssist<Obj, ObjFunPtr>::gMutex;

}

template <typename Ret, typename ...Args>
class Subject2
{
public:
    using Local = Subject2<Ret, Args...>;
    using Increment = Arrow::Other::TIncrement<Local, uintptr_t, 1>;
    using ObserverFunc=std::function<Ret(Args...)>; // 接收函数地址，建议使用std::bind创建对应function [zhuyb 2023-10-31 17:16:14]
    using MapKey = std::tuple<uintptr_t, uintptr_t>;
    using MapIDToObserverFunc = std::map<MapKey, ObserverFunc>;
    using FunPtr = Ret (*)(Args...);

public:
    Subject2(){}
    virtual ~Subject2(){}

    /**
     * @description: 注册回调函数。会根据对象地址和函数地址进行去重操作
     * @param {ObserverObj*} pObserver
     * @return {*}
     */
    template<typename ObserverObj>
    bool Register(ObserverObj* pObserver, Ret (ObserverObj::*pObjserverFunPtr)(Args...))
    {
        return RegisterClassFun(pObserver, pObjserverFunPtr, Arrow::MakeIntegerSequence_t<sizeof...(Args)>{});
    }

    /**
     * @description: 注册函数。不做去重处理
     * @param {ObserverFunc} pFun
     * @return {*}
     */
    bool Register(ObserverFunc pFun)
    {
        MapKey key = std::make_tuple(Increment::Get(), 0);
        std::lock_guard<std::shared_timed_mutex> guard(m_Mutex);
        m_mapObserver[key] = pFun;
        return true;
    }

    /**
     * @description: 反注册
     * @param {ObserverObj*} pObserver
     * @return {*}
     */
    template<typename ObserverObj>
    bool UnRegister(ObserverObj* pObserver, Ret (ObserverObj::*pObjserverFunPtr)(Args...))
    {
        return UnRegisterClassFun(pObserver, pObjserverFunPtr, Arrow::MakeIntegerSequence_t<sizeof...(Args)>{});
    }

    virtual bool UnRegister()
    {
        std::lock_guard<std::shared_timed_mutex> guard(m_Mutex);
        m_mapObserver.clear();
        return true;
    }

    virtual Ret Notify(Args... args)
    {
        m_Mutex.lock_shared();       
        for(auto& it :m_mapObserver)
        {
            it.second(args...);
        }

        m_Mutex.unlock_shared();
    }

private:
    template<typename ObserverObj, int ...sequence>
    bool RegisterClassFun(ObserverObj* pObserver, Ret (ObserverObj::*pObjserverFunPtr)(Args...), Arrow::IntegerSequence<sequence...>)
    {
        uintptr_t ptrObj = reinterpret_cast<uintptr_t>(pObserver);
        // uintptr_t ptrFun = reinterpret_cast<uintptr_t>(reinterpret_cast<char*>(pObjserverFunPtr));
        uintptr_t ptrFun = static_cast<uintptr_t>(details::ObserverAssist<ObserverObj, decltype(pObjserverFunPtr)>::GetID(pObjserverFunPtr));
        auto key = std::make_tuple(ptrObj, ptrFun);

        std::lock_guard<std::shared_timed_mutex> guard(m_Mutex);
        auto it = m_mapObserver.find(key);
        if(it != m_mapObserver.end())
        {
            return false;
        }
#if defined(__clang__)
    ObserverFunc fun = std::bind(pObjserverFunPtr, pObserver, std::placeholders::__ph<sequence+1>()...);
#elif defined(__GNUC__) && !defined(__clang__)
    ObserverFunc fun = std::bind(pObjserverFunPtr, pObserver, std::_Placeholder<sequence+1>()...);
#endif

        
        
        m_mapObserver.insert(std::make_pair(key, fun));
        return true;
    }

    template<typename ObserverObj, int ...sequence>
    bool UnRegisterClassFun(ObserverObj* pObserver, Ret (ObserverObj::*pObjserverFunPtr)(Args...), Arrow::IntegerSequence<sequence...>)
    {
        uintptr_t ptrObj = reinterpret_cast<uintptr_t>(pObserver);
        uintptr_t ptrFun = reinterpret_cast<uintptr_t>(reinterpret_cast<void*>(pObjserverFunPtr));
        auto key = std::make_tuple(ptrObj, ptrFun);

        std::lock_guard<std::shared_timed_mutex> guard(m_Mutex);
        auto it = m_mapObserver.find(key);
        if(it == m_mapObserver.end())
        {
            return false;
        }
        m_mapObserver.erase(it);
        return true;
    }

    // template<int ...sequence>
    // bool RegisterFun(FunPtr pFun, Arrow::IntegerSequence<sequence...>)
    // {
    //     uintptr_t ptrObj = nullptr;
    //     uintptr_t ptrFun = reinterpret_cast<uintptr_t>(pFun);
    //     auto key = std::make_tuple(ptrObj, ptrFun);

    //     std::lock_guard<std::shared_timed_mutex> guard(m_Mutex);
    //     auto it = m_mapObserver.find(key);
    //     if(it != m_mapObserver.end())
    //     {
    //         return false;
    //     }
    //     ObserverFunc fun = pFun;
    //     m_mapObserver.insert(std::make_pair(key, fun));
    //     return true;
    // }

protected:
    std::shared_timed_mutex m_Mutex;
    MapIDToObserverFunc m_mapObserver;
};

}
}