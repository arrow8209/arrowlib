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

    auto tp1 = std::chrono::steady_clock::now();
    for (std::size_t i = 0; i < szMax; ++i)
    {
        void* p = Arrow2::Other::AppSmallMemAllocator::Instance()->Allocate(512);
        Arrow2::Other::AppSmallMemAllocator::Instance()->Deallocate(p, 512);
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