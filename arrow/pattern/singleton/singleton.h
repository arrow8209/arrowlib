#pragma once
#include <memory>
#include <mutex>
namespace Arrow
{

namespace Pattern
{
// 该单例为自动单例，由操作系统自动回收 [zhuyb 2022-07-06 18:06:14]
template <typename _TInstance>
class Singleton
{
   
public:
    using TInstance = _TInstance;
    using Unique_Ptr_Instance = std::unique_ptr<TInstance>;

private:
    Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    
    virtual ~Singleton() = default;
public:

    static Unique_Ptr_Instance& Instance()
    {
        if (s_UniquePtrInstance.get() == nullptr)
        {
            s_Mutex.lock();
            if (s_UniquePtrInstance.get() == nullptr)
            {
                s_UniquePtrInstance.reset(new TInstance());
            }
            s_Mutex.unlock();
        }
        return s_UniquePtrInstance;
    }
protected:
    static Unique_Ptr_Instance s_UniquePtrInstance;
    static std::mutex s_Mutex;
};

template <typename TYPE>
typename Singleton<TYPE>::Unique_Ptr_Instance Singleton<TYPE>::s_UniquePtrInstance;

template <typename TYPE>
std::mutex Singleton<TYPE>::s_Mutex;

//Demo
//typedef Singleton<UserClass> App_UserClass;
}
}