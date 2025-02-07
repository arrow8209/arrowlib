#pragma once
#undef BOOST_LOCKFREE_PTR_COMPRESSION
#include <boost/lockfree/queue.hpp>

void TestBoostFreeLock()
{
    int nTmp1 = 0;
    boost::lockfree::queue<int*> q(10);
    q.push(&nTmp1);

    int* pTmp2 = nullptr;
    q.pop(pTmp2);
    std::cout << *pTmp2 << std::endl;
}