#pragma once
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>

namespace Arrow
{

namespace Pattern
{
template<typename ...Args>
class Handler
{
private:
    using Local = Handler<Args...>;
protected:
    Handler() {}

public:
    virtual ~Handler() {}

    /// @brief 
    /// @param ...args 
    /// @return 
    virtual void Handle(Args... args) = 0;

    void Push(Args... args)
    {
        if(m_pNext == nullptr)
            return;
        m_pNext->Handle(args...);
    }

    Local* Next(Local* pNext)
    {
        std::lock_guard<std::mutex> guard(m_Mutex);
        Local* pRet = m_pNext;
        m_pNext = pNext;
        return pRet;
    }

    Local* Next()
    {
        m_Mutex.lock();
        Local* pRet = m_pNext;
        m_Mutex.unlock();
        return pRet;
    }

private:
    std::mutex m_Mutex;
    Local* m_pNext = nullptr;
};

}
}
 