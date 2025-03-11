#pragma once

namespace Arrow
{

namespace Other
{

namespace detail
{
template<typename ...Args>
struct DeleteArgsImpl;

template<typename T, typename ...Args>
struct DeleteArgsImpl<T,  Args...>
{
    static void Del(T& val, Args&... args)
    {   
        DeleteArgsImpl<Args...>::Del(args...);
    }
};

template<typename T, typename ...Args>
struct DeleteArgsImpl<T*,  Args...>
{
    static void Del(T*& pObj, Args&... args)
    {   
        DeleteArgsImpl<T*>::Del(pObj);
        DeleteArgsImpl<Args...>::Del(args...);
    }
};

template<typename T>
struct DeleteArgsImpl<T>
{
    static void Del(T& val)
    {   
    }
};

template<typename T>
struct DeleteArgsImpl<T*>
{
    static void Del(T*& pObj)
    {   
        delete pObj;
    }
};

template<typename T>
struct DeleteArgsImpl<T[]>
{
    static void Del(T*& pObj)
    {   
        if (pObj != nullptr) 
        {
            delete[] pObj;
            pObj = nullptr;
        }
    }
};
}

template<typename ...Args>
void DeleteArgs(Args&... args)
{
    detail::DeleteArgsImpl<Args...>::Del(args...);
}

}
}