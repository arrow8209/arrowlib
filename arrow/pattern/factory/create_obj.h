#pragma once

namespace Arrow
{
namespace Other
{

template <typename T>
struct CreateUseNew
{
    static T* Create()
    {
        return new T();
    }

    static void Destory(T* ptr)
    {
        delete ptr;
    }
};

template<typename T>
struct CreateUseMalloc
{
    static T* Create()
    {
        void* pMem = malloc(sizeof(T));
        return new(pMem)T();
    }

    static void Destory(T* ptr)
    {
        ptr->~T();
        free(static_cast<void*>(ptr));
        void* pMem = ptr;
    }
};

}
}