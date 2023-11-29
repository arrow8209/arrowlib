/*
 * @FilePath: /arrowlib/arrow/other/obj_pool/obj_pool_simple2.h
 * @Author: arrow arrow8209@foxmail.com
 * @Date: 2022-12-07 10:08:16
 * @Description: 简单对象池。使用malloc/free 创建和销毁内存
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
#include "obj_pool_enum.h"

namespace Arrow
{

namespace Other
{

template<typename TObj, int livetime = _em_ObjLive120>
class TObjSimplePool2
{
    typedef std::tuple<void*, std::chrono::steady_clock::time_point> ObjTupleInfo;
    typedef std::list<ObjTupleInfo> Queue_ObjInfo;
    typedef std::map<void*, ObjTupleInfo> Map_ObjInfo;

    constexpr static uint32_t gObjSize = sizeof(TObj);
public:
    TObjSimplePool2()
    {
    }
    virtual ~TObjSimplePool2()
    {
        Release();
    }

    TObj* Alloc()
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        
        if (m_FreeObj.size() > 0)
        {
            ObjTupleInfo& objInfo = m_FreeObj.front();
            
            void* pBuf = std::get<0>(objInfo);
            m_ActiveObj[pBuf] = objInfo;

            new (pBuf)TObj();
            m_FreeObj.pop_front();
            return reinterpret_cast<TObj*>(pBuf);
        }

        void* pBuf = malloc(gObjSize);
        new (pBuf)TObj();

        ++m_n32PoolCount;
        ObjTupleInfo objInfo(pBuf, std::chrono::steady_clock::now());
        m_ActiveObj[pBuf] = objInfo;
        
        return reinterpret_cast<TObj*>(pBuf);
    }

    void Free(TObj* pObj)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        void* pBuf = reinterpret_cast<void*>(pObj);

        auto it = m_ActiveObj.find(pBuf);
        if (it == m_ActiveObj.end())
        {
            printf("[Arrow:Other:Pool][Warn]TObjSimplePool2<%s> Unkonw %p ptr\n", typeid(pObj).name(), pObj);
            delete pObj;
            return;
        }

        std::get<1>(it->second) = std::chrono::steady_clock::now();
        pObj->~TObj();

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
                free(std::get<0>(objInfo));
                m_n32PoolCount--;
            }
            catch (const std::exception& e)
            {
                printf("[Arrow:Other:Pool][Warn]TObjSimplePool2<%s> Destory Fail! Error Code:%s\n",typeid(TObj).name(),  e.what());
            }
        }
        auto it = m_ActiveObj.begin();
        void* pBuf = nullptr;
        for (; it != m_ActiveObj.end(); ++it)
        {
            try
            {
                pBuf = it->first;
                TObj* pObj = reinterpret_cast<TObj*>(pBuf);
                pObj->~TObj();
                free(pBuf);
                m_n32PoolCount--;
            }
            catch (const std::exception& e)
            {
                printf("[Arrow:Other:Pool][Warn]TObjSimplePool2<%s> Destory Fail! Error Code:%s\n",typeid(TObj).name(), e.what());
            }
        }
        
        m_ActiveObj.clear();
    }
protected:
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
                free(std::get<0>(*it));
                it = m_FreeObj.erase(it);
                --m_n32PoolCount;
            }
            else
            {
                ++it;
                continue;
            }
        }
        printf("[Arrow:Other:Pool][Info]TObjSimplePool2<%s> Pool Count:%u\n", typeid(TObj).name(), m_n32PoolCount.load());
    }

protected:
    std::mutex m_Mutex;
    Queue_ObjInfo m_FreeObj;
    Map_ObjInfo m_ActiveObj;
    std::chrono::steady_clock::time_point m_LastCheckObjTimePoint;

    std::atomic_int32_t m_n32PoolCount{0};
};
}
}