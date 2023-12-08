#include <vector>
#include "test/test_chunk.h"
#include "test/test_fixed_allocator.h"
#include "test/test_small_obj.h"
#include "test/test_performance.h"
#include "test/test_performance2.h"

int main(int argc, char* argv[])
{
    // TestChunk();
    // TestFixedAllocator();
    // TestSmallObjAllocator();
    // TestPerformance();
    TestPerformance2();
}