#pragma once
#include "arrow/task/lock_free_queue.h"

void TestLockFree1()
{
    Arrow::Pattern::LockFreeQueue<int, float> lockFreeQueue;
    lockFreeQueue.Push((int)1, float(1.1));
    lockFreeQueue.Push((int)2, float(1.2));
    lockFreeQueue.Push((int)3, float(1.3));
    lockFreeQueue.Push((int)4, float(1.4));
    lockFreeQueue.Push((int)5, float(1.5));

    int n1 = 0;
    float f1 = 0;
    bool bRet = true;
    bRet = lockFreeQueue.Pop(n1, f1);
    bRet = lockFreeQueue.Pop(n1, f1);
    bRet = lockFreeQueue.Pop(n1, f1);
    bRet = lockFreeQueue.Pop(n1, f1);
    bRet = lockFreeQueue.Pop(n1, f1);
    bRet = lockFreeQueue.Pop(n1, f1);
    bRet = lockFreeQueue.Pop(n1, f1);

    std::cout << lockFreeQueue.Size() << std::endl;
    std::cout << lockFreeQueue.Empty() << std::endl;
}