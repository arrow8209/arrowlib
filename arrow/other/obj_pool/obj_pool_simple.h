/*
 * @FilePath: /arrowlib/arrow/other/obj_pool/obj_pool_simple.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-12-07 10:08:16
 * @Description: 简单对象池。注意使用对象池的对象必须实现Release 接口。
 */
#pragma once
#include <queue>
#include <mutex>
#include <list>
#include <typeinfo>
#include <chrono>
#include <typeinfo>
#include <atomic>
#include <functional>
#include <map>
#include "allocator.h"

namespace Arrow
{

namespace Other
{

namespace details
{
/**
 * @description: 检测U是否存在Release 函数
 * @return {*}
 */
template <typename U>
struct checkClassReleaseNotParam
{
    template <typename T,
              typename Ret = typename std::reference_wrapper<decltype(&T::Release)>::result_type, // 萃取出返回值 [zhuyb// 2023-03-06 11:50:22],
              Ret (T::*)() = &T::Release>
    static constexpr bool check(T*)
    {
        return true;
    };
    static constexpr bool check(...) { return false; };
    static constexpr bool value = check(static_cast<U *>(0));
};
}

template<typename TObj, int livetime = _em_ObjLive120, typename Allocator = Allocator<TObj>>
class TObjSimplePool
{
    using ObjTupleInfo = std::tuple<TObj*, std::chrono::steady_clock::time_point> ;
    using Queue_ObjInfo = std::list<ObjTupleInfo> ;
    using Map_ObjInfo = std::map<TObj*, ObjTupleInfo> ;
public:
    TObjSimplePool()
    {
    }
    virtual ~TObjSimplePool()
    {
        Release();
    }

    TObj* Alloc()
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        
        TObj* pObj = nullptr;

        if (m_FreeObj.size() > 0)
        {
            ObjTupleInfo& objInfo = m_FreeObj.front();
            
            pObj = std::get<0>(objInfo);
            m_ActiveObj[pObj] = objInfo;

            m_FreeObj.pop_front();

            m_Allocator.Construct(pObj);
            return pObj;
        }

        pObj = m_Allocator.Alloc();
        if (pObj == nullptr)
        {
            return nullptr;
        }
        m_Allocator.Construct(pObj);
        ++m_n32PoolCount;
        ObjTupleInfo objInfo(pObj, std::chrono::steady_clock::now());
        m_ActiveObj[pObj] = objInfo;
        
        return pObj;
    }

    void Free(TObj* pObj)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        auto it = m_ActiveObj.find(pObj);
        if (it == m_ActiveObj.end())
        {
            CallTObjRelease(pObj);
            printf("[Arrow:Other:Pool][Warn]TObjSimplePool<%s> Unkonw %p ptr\n", typeid(pObj).name(), pObj);
            m_Allocator.Destructor(pObj);
            m_Allocator.Free(pObj);
            return;
        }

        std::get<1>(it->second) = std::chrono::steady_clock::now();
        // pObj->Release();
        CallTObjRelease(pObj);
        m_Allocator.Destructor(pObj);

        m_FreeObj.push_front(it->second);
        m_ActiveObj.erase(it);
        Check();
    }

    void Release()
    {    
        std::lock_guard<std::mutex> guard(m_Mutex);
        ObjTupleInfo objInfo;
        while (m_FreeObj.size() > 0)
        {
            objInfo = m_FreeObj.front();
            m_FreeObj.pop_front();

            try
            {
                CallTObjRelease(std::get<0>(objInfo));
                m_Allocator.Free(std::get<0>(objInfo));
                m_n32PoolCount--;
            }
            catch (const std::exception& e)
            {
                printf("[Arrow:Other:Pool][Warn]TObjSimplePool<%s> Destory Fail! Error Code:%s\n",typeid(TObj).name(),  e.what());
            }
        }
        auto it = m_ActiveObj.begin();
        TObj* pObj = nullptr;
        for (; it != m_ActiveObj.end(); ++it)
        {
            try
            {
                pObj = it->first;
                CallTObjRelease(pObj);
                m_Allocator.Destructor(pObj);
                m_Allocator.Free(pObj);
                m_n32PoolCount--;
            }
            catch (const std::exception& e)
            {
                printf("[Arrow:Other:Pool][Warn]TObjSimplePool<%s> Destory Fail! Error Code:%s\n",typeid(TObj).name(), e.what());
            }
        }
        
        m_ActiveObj.clear();
    }
protected:
    template <typename T = TObj,
              typename std::enable_if<details::checkClassReleaseNotParam<T>::value, T>::type* = nullptr>
    void CallTObjRelease(T* pObj)
    {
        pObj->Release();
    }

    template <typename T = TObj,
              typename std::enable_if<!details::checkClassReleaseNotParam<T>::value, T>::type* = nullptr>
    void CallTObjRelease(T* pObj)
    {
    }

    void Check()
    {
        if (livetime == 0)
        {
            return;
        }
        
        std::chrono::steady_clock::time_point tmNow = std::chrono::steady_clock::now();
        auto spacetime = std::chrono::duration_cast<std::chrono::seconds>(tmNow - m_LastCheckObjTimePoint);
        if (spacetime.count() < livetime)
        {
            return;
        }
        m_LastCheckObjTimePoint = tmNow;

        for (auto it = m_FreeObj.begin(); it != m_FreeObj.end(); )
        {
            spacetime = std::chrono::duration_cast<std::chrono::seconds>(tmNow - std::get<1>(*it));
            if (spacetime.count() > livetime)
            {
                m_Allocator.Free(std::get<0>(*it));
                it = m_FreeObj.erase(it);
                --m_n32PoolCount;
            }
            else
            {
                ++it;
                continue;
            }
        }
        printf("[Arrow:Other:Pool][Info]TObjSimplePool<%s> Pool Count:%u\n", typeid(TObj).name(), m_n32PoolCount.load());
    }

protected:
    std::mutex m_Mutex;
    Queue_ObjInfo m_FreeObj;
    Map_ObjInfo m_ActiveObj;
    std::chrono::steady_clock::time_point m_LastCheckObjTimePoint;

    std::atomic_int32_t m_n32PoolCount{0};
    Allocator m_Allocator;
};
}
}