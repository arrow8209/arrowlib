#pragma once
#include <queue>
#include <mutex>
#include <list>
#include <typeinfo>
#include <chrono>
#include <typeinfo>
#include <atomic>

namespace Arrow
{

namespace Other
{

// 对象池空闲存活时间 单位:秒 [zhuyb 2022-10-24 08:57:04]
typedef enum _em_ObjLiveTime
{
    _em_objLiveMax = 0,
    _em_ObjLiv30 = 30,
    _em_ObjLive60 = 60,
    _em_ObjLive90 = 90,
    _em_ObjLive120 = 120,
    _em_ObjLive240 = 240,
    _em_ObjLive480 = 480,
    _em_ObjLive960 = 960,
    _em_ObjLive1920 = 1920
} Em_ObjLiveTime;

template<typename TObj, int livetime = _em_ObjLive120>
class TObjSimplePool
{
    typedef std::tuple<TObj*, std::chrono::system_clock::time_point> ObjTupleInfo;
    typedef std::list<ObjTupleInfo> Queue_ObjInfo;
    typedef std::map<TObj*, ObjTupleInfo> Map_ObjInfo;
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
            return pObj;
        }

        pObj = new TObj();
        if (pObj == nullptr)
        {
            return nullptr;
        }

        ++m_n32PoolCount;
        ObjTupleInfo objInfo(pObj, std::chrono::system_clock::now());
        m_ActiveObj[pObj] = objInfo;
        
        return pObj;
    }

    void Free(TObj* pObj)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        auto it = m_ActiveObj.find(pObj);
        if (it == m_ActiveObj.end())
        {
            pObj->Release();
            printf("[Arrow:Other:Pool][Warn]TObjSimplePool<%s> Unkonw %p ptr\n", typeid(pObj).name(), pObj);
            delete pObj;
            return;
        }

        std::get<1>(it->second) = std::chrono::system_clock::now();
        pObj->Release();
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
                std::get<0>(objInfo)->Release();
                delete std::get<0>(objInfo);
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
                pObj->Release();
                delete pObj;
                m_n32PoolCount--;
            }
            catch (const std::exception& e)
            {
                printf("[Arrow:Other:Pool][Warn]TObjSimplePool<%s> Destory Fail! Error Code:%s\n",typeid(TObj).name(), e.what());
            }
        }
        
        m_ActiveObj.clear();
    }

    private:
    void Check()
    {
        if (livetime == 0)
        {
            return;
        }
        

        std::chrono::system_clock::time_point tmNow = std::chrono::system_clock::now();
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
                delete std::get<0>(*it);
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
    std::chrono::system_clock::time_point m_LastCheckObjTimePoint;

    std::atomic_int32_t m_n32PoolCount{0};
};
}
}