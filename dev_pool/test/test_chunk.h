#pragma once
#include <list>
#include <random>
#include "../src/chunk.h"

void TestChunk()
{
    // 创建一个随机数引擎
    std::random_device rd;
    std::mt19937 generator(rd());

    Arrow2::Other::Chunk chunk;
    chunk.Init(16, 15);

    std::vector<int32_t*> vecIntPtr;

    int32_t* pInt = nullptr;
    int32_t n32Index = 0;
    int32_t n32Tmp = 0;
    chunk.MemInfo();
    for (int i = 0; i < 100000; i++)
    {
        if (vecIntPtr.size() <= 2)
        {
            for (int i = 0; vecIntPtr.size() < 10; ++i)
            {
                pInt = static_cast<int32_t*>(chunk.Allocate());
                vecIntPtr.push_back(pInt);
            }
        }
        else if (vecIntPtr.size() >= 10)
        {
            std::uniform_int_distribution<int> distribution(0, vecIntPtr.size() - 1);
            n32Index = distribution(generator);
            pInt = vecIntPtr[n32Index];
            vecIntPtr.erase(vecIntPtr.begin() + n32Index);
            chunk.Deallocate(pInt);
        }
        else
        {
            std::uniform_int_distribution<int> distribution(0, vecIntPtr.size() - 1);
            n32Index = distribution(generator);
            if (n32Index % 2 == 0)
            {
                pInt = static_cast<int32_t*>(chunk.Allocate());
                vecIntPtr.push_back(pInt);
            }
            else
            {
                pInt = vecIntPtr[n32Index];
                vecIntPtr.erase(vecIntPtr.begin() + n32Index);
                chunk.Deallocate(pInt);
            }
        }
        chunk.MemInfo();
    }
}