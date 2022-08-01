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
    typedef _TInstance TInstance;
    typedef std::unique_ptr<TInstance> Unique_Ptr_Instance;

private:
    Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    
    virtual ~Singleton() = default;
public:

    
    static Unique_Ptr_Instance& Instance()
    {
        static bool bCreate = false;
        static std::mutex mutext;
        if (bCreate == false)
        {
            mutext.lock();
            if (s_UniquePtrInstance.get() == NULL)
            {
                s_UniquePtrInstance.reset(new TInstance);
            }
            bCreate = true;
            mutext.unlock();
        }
        return s_UniquePtrInstance;
    }
protected:
    static Unique_Ptr_Instance s_UniquePtrInstance;
};

template<typename TYPE> typename Singleton<TYPE>::Unique_Ptr_Instance Singleton<TYPE>::s_UniquePtrInstance;

//Demo
//typedef Singleton<UserClass> App_UserClass;
}
}