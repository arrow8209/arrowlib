#include <vector>
#include "test_chunk.h"
#include "test_fixed_allocator.h"
#include "test_small_obj.h"

int main(int argc, char* argv[])
{
    std::cout << sizeof(std::vector<int32_t>) << std::endl;
    std::cout << sizeof(std::list<int32_t>) << std::endl;
    std::cout << sizeof(std::tuple<int32_t,int32_t>) << std::endl;
    std::cout << sizeof(std::map<int32_t,int32_t>) << std::endl;
    std::cout << sizeof(std::string) << std::endl;
    // TestChunk();
    // TestFixedAllocator();
    TestSmallObjAllocator();
    return 0;
}