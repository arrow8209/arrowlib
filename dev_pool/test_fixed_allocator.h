#pragma once
#include "fixed_allocator.h"

void TestFixedAllocator()
{
    // 创建一个随机数引擎
    std::random_device rd;
    std::mt19937 generator(rd());

    Arrow2::Other::FixedAllocator allocator;
    allocator.Initialize(32, 160);

    std::vector<int32_t*> vecIntPtr;
    int32_t* pInt = nullptr;
    int32_t n32Index = 0;
    int32_t n32Tmp = 0;

    for (int i = 0; i < 100000; i++)
    {
        if (vecIntPtr.size() <= 2)
        {
            std::cout << "Allocate" << std::endl;
            for (int i = 0; vecIntPtr.size() < 10; ++i)
            {
                pInt = static_cast<int32_t*>(allocator.Allocate());
                vecIntPtr.push_back(pInt);
            }
        }
        else if (vecIntPtr.size() >= 10)
        {
            std::cout << "Deallocate" << std::endl;
            std::uniform_int_distribution<int> distribution(0, vecIntPtr.size() - 1);
            n32Index = distribution(generator);
            pInt = vecIntPtr[n32Index];
            vecIntPtr.erase(vecIntPtr.begin() + n32Index);
            allocator.Deallocate(pInt, nullptr);
        }
        else
        {
            std::uniform_int_distribution<int> distribution(0, vecIntPtr.size() - 1);
            n32Index = distribution(generator);
            if (n32Index % 2 == 0)
            {
                std::cout << "Allocate" << std::endl;
                pInt = static_cast<int32_t*>(allocator.Allocate());
                vecIntPtr.push_back(pInt);
            }
            else
            {
                std::cout << "Deallocate" << std::endl;
                pInt = vecIntPtr[n32Index];
                vecIntPtr.erase(vecIntPtr.begin() + n32Index);
                allocator.Deallocate(pInt, nullptr);
            }
        }
        allocator.MemInfo();
        std::cout << std::endl;
    }
}