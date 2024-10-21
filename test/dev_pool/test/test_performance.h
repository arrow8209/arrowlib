#pragma once
#include <vector>
#include <chrono>
#include <random>
#include "arrow/arrow.h"
#include "../src/small_obj_allocator.h"

template<typename T>
class Allocator2
{
public:
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using const_pointer = const T*;
    using value_type = T;

public:
    Allocator2()
    {
    }

    ~Allocator2()
    {
    }

    /**
     * @description: 分配内存
     * @return {*}
     */
    inline pointer Alloc()
    {
        // ::operator new 调用系统new函数，只分配内存不调用构造函数 [zhuyb 2023-12-04 17:56:34]
        return static_cast<pointer>(Arrow2::Other::AppSmallMemAllocator::Instance()->Allocate(sizeof(T)));
    }

    /**
     * @description: 回收内存
     * @param {pointer} ptr
     * @return {*}
     */
    inline void Free(pointer ptr)
    {
        // ::operator delete 调用系统delete函数，回收内存不调用析构函数[zhuyb 2023-12-04 17:57:13]
        // ::operator delete(ptr);
        return Arrow2::Other::AppSmallMemAllocator::Instance()->Deallocate(static_cast<void*>(ptr), sizeof(T));
    }

    /**
     * @description: 调用构造函数
     * @param {pointer} ptr
     * @return {*}
     */
    inline void Construct(pointer ptr)
    {
        ::new ((void*) ptr) T();
    }

    /**
     * @description: 调用析构函数
     * @param {pointer} ptr
     * @return {*}
     */
    inline void Destructor(pointer ptr)
    {
        ptr->~T();
    }
};

class CTestPerformance //: public Arrow2::Other::SmallObjectBase
{
public:
	CTestPerformance()
	{
		// printf("CTestPerformance()\n");
	}
    virtual ~CTestPerformance()
    {
        // printf("~CTestPerformance()\n");
    }
protected:
    std::vector<int> m_vec;
};

using TestPerformancePool1 = Arrow::Other::TObjSimplePool<CTestPerformance, Arrow::Other::_em_ObjLiv30, Allocator2<CTestPerformance>>;
using AppTestPerformancePool1 = Arrow::Pattern::Singleton<TestPerformancePool1>;

using TestPerformancePool2 = Arrow::Other::TObjSimplePool<CTestPerformance, Arrow::Other::_em_ObjLiv30, Arrow::Other::Allocator<CTestPerformance>>;
using AppTestPerformancePool2 = Arrow::Pattern::Singleton<TestPerformancePool2>;

void TestPerformance()
{
    // 创建一个随机数引擎
    std::size_t szMax = 100000;
    std::vector<CTestPerformance*> vec;
    vec.resize(szMax / 2);

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int32_t> distribution(1, szMax / 2);
    int32_t n32Index = 0;

    int32_t nCreate1 = 0;
    int32_t nDel1 = 0;
    int32_t nCreate2 = 0;
    int32_t nDel2 = 0;
    int32_t nCreate3 = 0;
    int32_t nDel3 = 0;

    auto tp1 = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < szMax; ++i)
    {
        n32Index = distribution(generator);

        if (vec[n32Index - 1] != nullptr)
        {
            AppTestPerformancePool1::Instance()->Free(vec[n32Index - 1]);
            vec[n32Index - 1] = nullptr;
            nDel1++;
        }
        else
        {
            vec[n32Index - 1] = AppTestPerformancePool1::Instance()->Alloc();
            nCreate1++;
        }
    }
    auto tp2 = std::chrono::steady_clock::now();

    for(std::size_t i = 0; i < vec.size(); ++i)
    {
        if (vec[i] != nullptr)
        {
            AppTestPerformancePool1::Instance()->Free(vec[i]);
            vec[i] = nullptr;
        }
    }

    auto tp3 = std::chrono::steady_clock::now();
    for(std::size_t i = 0; i < szMax; ++i)
    {
        n32Index = distribution(generator);
        if (vec[n32Index - 1] != nullptr)
        {
            AppTestPerformancePool2::Instance()->Free(vec[n32Index - 1]);
            vec[n32Index - 1] = nullptr;
            nDel2++;
        }
        else
        {
            vec[n32Index - 1] = AppTestPerformancePool2::Instance()->Alloc();
            nCreate2++;
        }
    }
    auto tp4 = std::chrono::steady_clock::now();

    for (std::size_t i = 0; i < vec.size(); ++i)
    {
        if (vec[i] != nullptr)
        {
            AppTestPerformancePool2::Instance()->Free(vec[i]);
            vec[i] = nullptr;
        }
    }

    auto tp5 = std::chrono::steady_clock::now();
    for(std::size_t i = 0; i < szMax; ++i)
    {
        n32Index = distribution(generator);
        if (vec[n32Index - 1] != nullptr)
        {
            delete vec[n32Index - 1];
            vec[n32Index - 1] = nullptr;
            nDel3++;
        }
        else
        {
            vec[n32Index - 1] = new CTestPerformance();
            nCreate3++;
        }
    }
    auto tp6 = std::chrono::steady_clock::now();

    for (std::size_t i = 0; i < vec.size(); ++i)
    {
        if (vec[i] != nullptr)
        {
            delete vec[i];
            vec[i] = nullptr;
        }
    }


    std::cout << "Chunk:" << std::chrono::duration_cast<std::chrono::milliseconds>(tp2 - tp1).count() << " " << nCreate1 << ":" << nDel1 << " " << std::endl;
    std::cout << "ObjPool:" << std::chrono::duration_cast<std::chrono::milliseconds>(tp4 - tp3).count()  << " " << nCreate2 << ":" << nDel2 << " "<< std::endl;
    std::cout << "new/del:" << std::chrono::duration_cast<std::chrono::milliseconds>(tp6 - tp5).count()  << " " << nCreate3 << ":" << nDel3 << " "<< std::endl;

    std::cout << "Chunk:" << 1.0 * std::chrono::duration_cast<std::chrono::microseconds>(tp2 - tp1).count() / nCreate1 << " "
             << "ObjPool:" << 1.0 * std::chrono::duration_cast<std::chrono::microseconds>(tp4 - tp3).count() / nCreate2 << " "
             << "new/del:" << 1.0 * std::chrono::duration_cast<std::chrono::microseconds>(tp6 - tp5).count() / nCreate3 << std::endl;

}