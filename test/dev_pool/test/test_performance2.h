#pragma once
#include <vector>
#include <chrono>
#include <random>
#include "arrow/arrow.h"
#include "../src/small_obj_allocator.h"


void TestPerformance2()
{
    // 创建一个随机数引擎
    std::size_t szMax = 100000;
    std::vector<void*> vec;
    vec.resize(szMax / 2);

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int32_t> distribution(1, szMax / 2);
    int32_t n32Index = 0;

    int32_t nCreate1 = 0;
    int32_t nDel1 = 0;
    int32_t nCreate2 = 0;
    int32_t nDel2 = 0;

    auto tp1 = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < szMax; ++i)
    {
        void* p = Arrow2::Other::AppSmallMemAllocator::Instance()->Allocate(512);
        Arrow2::Other::AppSmallMemAllocator::Instance()->Deallocate(p, 512);

        n32Index = distribution(generator);

        if (vec[n32Index - 1] != nullptr)
        {
            Arrow2::Other::AppSmallMemAllocator::Instance()->Deallocate(vec[n32Index - 1], 128);
            vec[n32Index - 1] = nullptr;
            nDel1++;
        }
        else
        {
            vec[n32Index - 1] = Arrow2::Other::AppSmallMemAllocator::Instance()->Allocate(128);
            nCreate1++;
        }
    }

    for (std::size_t i = 0; i < vec.size(); ++i)
    {
        if (vec[i] != nullptr)
        {
            Arrow2::Other::AppSmallMemAllocator::Instance()->Allocate(vec[i]);
            vec[i] = nullptr;
        }
    }
    auto tp2 = std::chrono::steady_clock::now();

    for(std::size_t i = 0; i < szMax; ++i)
    {
        char* p = new char[512];
        delete[] p;
    }
    auto tp3 = std::chrono::steady_clock::now();

    

    std::cout << "Chunk:" << 1.0 * std::chrono::duration_cast<std::chrono::microseconds>(tp2 - tp1).count() / szMax << " "
             << "new/del:" << 1.0 * std::chrono::duration_cast<std::chrono::microseconds>(tp3 - tp2).count() / szMax << std::endl;

}