#pragma once
#include <memory>
#include <mutex>
namespace Arrow
{
namespace Pattern
{
// 该单例为手动单例，需要显示调用Release释放单例使用的内存 [zhuyb 2022-07-06 18:06:14]
template <typename _TInstance>
class Singleton_Unmanager
{
    // friend class _TInstance;

public:
    typedef _TInstance TInstance;

private:
    Singleton_Unmanager() = default;
    Singleton_Unmanager(const Singleton_Unmanager&) = delete;
    Singleton_Unmanager& operator=(const Singleton_Unmanager&) = delete;

    virtual ~Singleton_Unmanager() = default;

public:
    static TInstance* Instance()
    {
        // if (s_bICreate == false)
        // {
        //     s_MutextInstance.lock();
        //     if (s_pInstance == nullptr)
        //     {
        //         s_pInstance = new TInstance();
        //     }
        //     s_bICreate = true;
        //     s_MutextInstance.unlock();
        // }
        // return s_pInstance;

        if (s_pInstance == nullptr)
        {
            s_MutextInstance.lock();
            if (s_pInstance == nullptr)
            {
                s_pInstance = new TInstance();
            }
            s_MutextInstance.unlock();
        }
        return s_pInstance;
    }

    static void Release()
    {
        if (s_pInstance != nullptr)
        {
            s_MutextInstance.lock();
            if (s_pInstance != nullptr)
            {
                delete s_pInstance;
                s_pInstance = nullptr;
            }
            s_MutextInstance.unlock();
        }
    }

protected:
    static TInstance* s_pInstance;
    static std::mutex s_MutextInstance;
    static bool s_bICreate;
};

template <typename _TInstance>
_TInstance* Singleton_Unmanager<_TInstance>::s_pInstance = nullptr;

template <typename _TInstance>
std::mutex Singleton_Unmanager<_TInstance>::s_MutextInstance;

template <typename _TInstance>
bool Singleton_Unmanager<_TInstance>::s_bICreate = false;

// Demo
// typedef Singleton<UserClass> App_UserClass;
} 
}