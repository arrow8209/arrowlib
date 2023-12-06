#pragma once
#include <assert.h>
#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <stdint.h>
#include <vector>
#include "arrow/arrow.h"
#include "chunk.h"
#include "fixed_allocator.h"

namespace Arrow2
{
namespace Other
{
#define USE_NEW_TO_ALLOCATE

#ifndef ARROW_OTHER_MAX_PAGE_SIZE 
#define ARROW_OTHER_MAX_PAGE_SIZE 4096
#endif

#ifndef ARROW_OTHER_MAX_OBJECT_SIZE
#define ARROW_OTHER_MAX_OBJECT_SIZE 512 
#endif

#ifndef ARROW_OTHER_OBJECT_ALIGN_SIZE
#define ARROW_OTHER_OBJECT_ALIGN_SIZE 8
#endif

/**
 * @description: 
 * @param {size_t} PageSize 内存页大小
 * @param {size_t} MaxObjectSize 对象最大size
 * @param {size_t} ObjectAlignSize 内存块对齐字节数
 * @return {*}
 */
template <std::size_t PageSize = ARROW_OTHER_MAX_PAGE_SIZE, 
            std::size_t MaxObjectSize = ARROW_OTHER_MAX_OBJECT_SIZE, 
            std::size_t ObjectAlignSize = ARROW_OTHER_OBJECT_ALIGN_SIZE>
class SmallObjAllocator
{
private:

    /// Copy-constructor is not implemented.
    SmallObjAllocator(const SmallObjAllocator&);
    /// Copy-assignment operator is not implemented.
    SmallObjAllocator& operator=(const SmallObjAllocator&);

public:

    SmallObjAllocator(void) 
    {
        assert(0 != ObjectAlignSize);
        const std::size_t allocCount = GetOffset(MaxObjectSize);
        pool_ = new FixedAllocator[allocCount];
        for (std::size_t i = 0; i < allocCount; ++i)
            pool_[i].Initialize((i + 1) * ObjectAlignSize, PageSize);
    }

    /** Destructor releases all blocks, all Chunks, and FixedAllocator's.
     Any outstanding blocks are unavailable, and should not be used after
     this destructor is called.  The destructor is deliberately non-virtual
     because it is protected, not public.
     */
    ~SmallObjAllocator(void)
    {
        if(pool_ != nullptr)
            delete [] pool_;
        pool_ = nullptr;
    }

public:
    /**
     * @description: 获取对应的内存块索引
     * @param {size_t} numBytes 需要的内存字节数
     * @param {size_t} alignment 对齐字节数
     * @return {*}
     */
    inline static std::size_t GetOffset(std::size_t numBytes)
    {
        return (numBytes + ObjectAlignSize - 1) / ObjectAlignSize;
    }

    inline static void* DefaultAllocator(std::size_t numBytes)
    {
#ifdef USE_NEW_TO_ALLOCATE
        return ::operator new(numBytes);
#else
        void* p = ::std::malloc(numBytes);
        if (doThrow && (NULL == p))
            throw std::bad_alloc();
        return p;
#endif
    }

    inline static void DefaultDeallocator(void* p)
    {
#ifdef USE_NEW_TO_ALLOCATE
        ::operator delete(p);
#else
        ::std::free(p);
#endif
    }

    /** Allocates a block of memory of requested size.  Complexity is often
     constant-time, but might be O(C) where C is the number of Chunks in a
     FixedAllocator.

     @par Exception Safety Level
     Provides either strong-exception safety, or no-throw exception-safety
     level depending upon doThrow parameter.  The reason it provides two
     levels of exception safety is because it is used by both the nothrow
     and throwing new operators.  The underlying implementation will never
     throw of its own accord, but this can decide to throw if it does not
     allocate.  The only exception it should emit is std::bad_alloc.

     @par Allocation Failure
     If it does not allocate, it will call TrimExcessMemory and attempt to
     allocate again, before it decides to throw or return NULL.  Many
     allocators loop through several new_handler functions, and terminate
     if they can not allocate, but not this one.  It only makes one attempt
     using its own implementation of the new_handler, and then returns NULL
     or throws so that the program can decide what to do at a higher level.
     (Side note: Even though the C++ Standard allows allocators and
     new_handlers to terminate if they fail, the Loki allocator does not do
     that since that policy is not polite to a host program.)

     @param numBytes # of bytes needed for allocation.
     @param doThrow True if this should throw if unable to allocate, false
      if it should provide no-throw exception safety level.
     @return NULL if nothing allocated and doThrow is false.  Else the
      pointer to an available block of memory.
     */
    /**
     * @description: 分配内存，需要内存数大于最大对象字节数使用DefaultAllocator否则使用内存池中的内存
     * @param {size_t} numBytes 需要的内存字节数
     * @return {*}
     */
    void* Allocate(std::size_t numBytes)
    {
        // 如果需要的直接数大于预定义的最大对象字节数使用默认没存分配 [zhuyb 2023-12-06 15:51:23]
        if (numBytes > MaxObjectSize)
            return DefaultAllocator(numBytes);

        assert(NULL != pool_);
        if (0 == numBytes)
            numBytes = 1;
        const std::size_t index = GetOffset(numBytes) - 1;
        const std::size_t allocCount = GetOffset(MaxObjectSize);
        (void)allocCount;
        assert(index < allocCount);

        FixedAllocator& allocator = pool_[index];
        assert(allocator.BlockSize() >= numBytes);
        assert(allocator.BlockSize() < numBytes + ObjectAlignSize);
        void* place = allocator.Allocate();

        if ((NULL == place) && TrimExcessMemory())
            place = allocator.Allocate();

        if ((NULL == place))
        {
            throw std::bad_alloc();
        }
        return place;
    }

    /** Deallocates a block of memory at a given place and of a specific
    size.  Complexity is almost always constant-time, and is O(C) only if
    it has to search for which Chunk deallocates.  This never throws.
     */
    void Deallocate(void* p, std::size_t numBytes)
    {
        if (NULL == p)
            return;
        if (numBytes > MaxObjectSize)
        {
            DefaultDeallocator(p);
            return;
        }
        assert(NULL != pool_);
        if (0 == numBytes)
            numBytes = 1;
        const std::size_t index = GetOffset(numBytes) - 1;
        const std::size_t allocCount = GetOffset(MaxObjectSize);
        (void)allocCount;
        assert(index < allocCount);
        FixedAllocator& allocator = pool_[index];
        assert(allocator.BlockSize() >= numBytes);
        assert(allocator.BlockSize() < numBytes + ObjectAlignSize);
        const bool found = allocator.Deallocate(p, NULL);
        (void)found;
        assert(found);
    }

    /** Deallocates a block of memory at a given place but of unknown size
    size.  Complexity is O(F + C) where F is the count of FixedAllocator's
    in the pool, and C is the number of Chunks in all FixedAllocator's.  This
    does not throw exceptions.  This overloaded version of Deallocate is
    called by the nothow delete operator - which is called when the nothrow
    new operator is used, but a constructor throws an exception.
     */
    void Deallocate(void* p)
    {
        if (NULL == p)
            return;
        assert(NULL != pool_);
        FixedAllocator* pAllocator = NULL;
        const std::size_t allocCount = GetOffset(MaxObjectSize);
        Chunk* chunk = NULL;

        for (std::size_t i = 0; i < allocCount; ++i)
        {
            chunk = pool_[i].HasBlock(p);
            if (NULL != chunk)
            {
                pAllocator = &pool_[i];
                break;
            }
        }
        if (NULL == pAllocator)
        {
            DefaultDeallocator(p);
            return;
        }

        assert(NULL != chunk);
        const bool found = pAllocator->Deallocate(p, chunk);
        (void)found;
        assert(found);
    }

    /** Releases empty Chunks from memory.  Complexity is O(F + C) where F
    is the count of FixedAllocator's in the pool, and C is the number of
    Chunks in all FixedAllocator's.  This will never throw.  This is called
    by AllocatorSingleto::ClearExtraMemory, the new_handler function for
    Loki's allocator, and is called internally when an allocation fails.
    @return True if any memory released, or false if none released.
     */
    bool TrimExcessMemory(void)
    {
        bool found = false;
        const std::size_t allocCount = GetOffset(MaxObjectSize);
        
        for (std::size_t i = 0; i < allocCount; ++i)
        {
            if (pool_[i].TrimEmptyChunk())
                found = true;
        }
        for (std::size_t i = 0; i < allocCount; ++i)
        {
            if (pool_[i].TrimChunkList())
                found = true;
        }

        return found;
    }

    /** Returns true if anything in implementation is corrupt.  Complexity
     is O(F + C + B) where F is the count of FixedAllocator's in the pool,
     C is the number of Chunks in all FixedAllocator's, and B is the number
     of blocks in all Chunks.  If it determines any data is corrupted, this
     will return true in release version, but assert in debug version at
     the line where it detects the corrupted data.  If it does not detect
     any corrupted data, it returns false.
     */
    bool IsCorrupt(void) const
    {
        if (NULL == pool_)
        {
            assert(false);
            return true;
        }
        if (0 == ObjectAlignSize)
        {
            assert(false);
            return true;
        }
        if (0 == MaxObjectSize)
        {
            assert(false);
            return true;
        }
        const std::size_t allocCount = GetOffset(MaxObjectSize);
        for (std::size_t i = 0; i < allocCount; ++i)
        {
            if (pool_[i].IsCorrupt())
                return true;
        }
        return false;
    }

private:
    /// Pointer to array of fixed-size allocators.
    FixedAllocator* pool_ = nullptr;
};



using AppSmallObjAllocator = Arrow::Pattern::Singleton<SmallObjAllocator<>>;
}
}