#pragma once
#include "../src/small_obj_allocator.h"
#include "../src/small_obj_base.h"

class TestSmallClass1 : public Arrow2::Other::SmallObjectBase
{
public:
	TestSmallClass1()
	{
		printf("TestSmallClass1()\n");
	}
    virtual ~TestSmallClass1()
    {
        printf("~TestSmallClass1()\n");
    }
};

void TestSmallObjAllocator()
{
    TestSmallClass1* pSmallClass = new TestSmallClass1();
    delete pSmallClass;
}