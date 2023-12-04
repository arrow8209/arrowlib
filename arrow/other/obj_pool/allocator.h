#pragma once

namespace Arrow
{

namespace Other
{

template<typename T>
class Allocator
{
public:
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using value_type = T;

public:
    Allocator()
    {
    }

    ~Allocator()
    {
    }

    /**
     * @description: 分配内存
     * @return {*}
     */
    pointer Alloc()
    {
        // ::operator new 调用系统new函数，只分配内存不调用构造函数 [zhuyb 2023-12-04 17:56:34]
        return static_cast<pointer>(::operator new(sizeof(T)));
    }

    /**
     * @description: 回收内存
     * @param {pointer} ptr
     * @return {*}
     */
    void Free(pointer ptr)
    {
        // ::operator delete 调用系统delete函数，回收内存不调用析构函数[zhuyb 2023-12-04 17:57:13]
        ::operator delete(ptr);
    }

    /**
     * @description: 调用构造函数
     * @param {pointer} ptr
     * @return {*}
     */
    void Construct(pointer ptr)
    {
        ::new ((void*) ptr) T();
    }

    /**
     * @description: 调用析构函数
     * @param {pointer} ptr
     * @return {*}
     */
    void Destructor(pointer ptr)
    {
        ptr->~T();
    }
};

}
}