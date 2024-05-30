#pragma once
#include <assert.h>
#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <stdint.h>
#include <vector>
#include "chunk.h"

namespace Arrow2
{
namespace Other
{

/*
作用:
提供分配固定大小对象的服务。
有一个容器存放所有的“Chunk”。
说明：
1.总有0个或1个Chunk是空闲的
2.没有空闲的Chunk,emptyChunk为空
3.如果有一个空闲的Chunk,那么emptyChunk指向它
4.如果 Chunk容器为空,deallocChunk和allocChunk为空
5.如果Chunk容器不为空，deallocChunk和allocChunk为空或者指向容器内的Chunk
6.allocChunk通常指向容易得最后一个Chunk，它可是是最近分配的，因此可能有用户块。
*/

class FixedAllocator
{
    using VecChunk = std::vector<Chunk>;

public:
    FixedAllocator() {}

    ~FixedAllocator()
    {
#ifdef DO_EXTRA_LOKI_TESTS
        TrimEmptyChunk();
        assert(m_vecChunks.empty() && "Memory leak detected!");
#endif
        for (auto it = m_vecChunks.begin(); it != m_vecChunks.end(); ++it)
            it->Release();
    }

    // /// Not implemented.
    // FixedAllocator(const FixedAllocator&);
    // /// Not implemented.
    // FixedAllocator& operator=(const FixedAllocator&);

    /**
     * @description: 初始化
     * @param {size_t} blockSize 最小内存块大小
     * @param {size_t} pageSize 整块内存大小
     * @return {*}
     */
    void Initialize(std::size_t blockSize, std::size_t pageSize)
    {
        assert(blockSize > 0);
        assert(pageSize >= blockSize);
        m_blockSize = blockSize;

        std::size_t numBlocks = pageSize / blockSize;
        if (numBlocks > s_MaxChunkBlockCount)
            numBlocks = s_MaxChunkBlockCount;
        else if (numBlocks < s_MinChunkBlockCount)
            numBlocks = s_MinChunkBlockCount;

        m_numBlocks = static_cast<uint8_t>(numBlocks);
        assert(m_numBlocks == numBlocks);
    }

    /**
     * @description: 分配内存
     * @return {*}
     */
    void* Allocate(void)
    {
        // prove either m_emptyChunk points nowhere, or points to a truly empty Chunk.
        assert((NULL == m_emptyChunk) || (m_emptyChunk->HasAvailable(m_numBlocks)));
        assert(CountEmptyChunks() < 2);

        if ((NULL == m_allocChunk) || m_allocChunk->IsFilled())
        {
            if (NULL != m_emptyChunk)
            {
                m_allocChunk = m_emptyChunk;
                m_emptyChunk = NULL;
            }
            else
            {
                for (auto it = m_vecChunks.begin();; ++it)
                {
                    if (m_vecChunks.end() == it)
                    {
                        if (!MakeNewChunk())
                            return NULL;
                        break;
                    }
                    if (!it->IsFilled())
                    {
                        m_allocChunk = &*it;
                        break;
                    }
                }
            }
        }
        else if (m_allocChunk == m_emptyChunk)
            // detach m_emptyChunk from m_allocChunk, because after
            // calling m_allocChunk->Allocate(m_blockSize); the chunk
            // is no longer empty.
            m_emptyChunk = NULL;

        assert(m_allocChunk != NULL);
        assert(!m_allocChunk->IsFilled());
        void* place = m_allocChunk->Allocate();

        // prove either m_emptyChunk points nowhere, or points to a truly empty Chunk.
        assert((NULL == m_emptyChunk) || (m_emptyChunk->HasAvailable(m_numBlocks)));
        assert(CountEmptyChunks() < 2);
#ifdef LOKI_CHECK_FOR_CORRUPTION
        if (m_allocChunk->IsCorrupt(m_numBlocks, m_blockSize, true))
        {
            assert(false);
            return NULL;
        }
#endif

        return place;
    }

    /** Deallocate a memory block previously allocated with Allocate.  If
     the block is not owned by this FixedAllocator, it returns false so
     that SmallObjAllocator can call the default deallocator.  If the
     block was found, this returns true.
     */
    bool Deallocate(void* p, Chunk* hint)
    {
        assert(!m_vecChunks.empty());
        assert(&m_vecChunks.front() <= m_deallocChunk);
        assert(&m_vecChunks.back() >= m_deallocChunk);
        assert(&m_vecChunks.front() <= m_allocChunk);
        assert(&m_vecChunks.back() >= m_allocChunk);
        assert(CountEmptyChunks() < 2);

        Chunk* foundChunk = (NULL == hint) ? VicinityFind(p) : hint;
        if (NULL == foundChunk)
            return false;

        assert(foundChunk->HasBlock(p));
#ifdef LOKI_CHECK_FOR_CORRUPTION
        if (foundChunk->IsCorrupt(m_numBlocks, m_blockSize, true))
        {
            assert(false);
            return false;
        }
        if (foundChunk->IsBlockAvailable(p, m_numBlocks, m_blockSize))
        {
            assert(false);
            return false;
        }
#endif
        m_deallocChunk = foundChunk;
        DoDeallocate(p);
        assert(CountEmptyChunks() < 2);

        return true;
    }

    /// Returns block size with which the FixedAllocator was initialized.
    inline std::size_t BlockSize() const
    {
        return m_blockSize;
    }

    /** Releases the memory used by the empty Chunk.  This will take
     constant time under any situation.
     @return True if empty chunk found and released, false if none empty.
     */
    /**
     * @description: 释放 m_emptyChunk 的内存
     * @return {*} true:存在 m_emptyChunk并释放 false: m_emptyChunk 为空
     */
    bool TrimEmptyChunk(void)
    {
        // prove either m_emptyChunk points nowhere, or points to a truly empty Chunk.
        assert((NULL == m_emptyChunk) || (m_emptyChunk->HasAvailable(m_numBlocks)));
        if (NULL == m_emptyChunk)
            return false;

        // If m_emptyChunk points to valid Chunk, then chunk list is not empty.
        assert(!m_vecChunks.empty());
        // And there should be exactly 1 empty Chunk.
        assert(1 == CountEmptyChunks());

        Chunk* lastChunk = &m_vecChunks.back();
        if (lastChunk != m_emptyChunk)
            std::swap(* m_emptyChunk, *lastChunk);
        assert(lastChunk->HasAvailable(m_numBlocks));
        lastChunk->Release();
        m_vecChunks.pop_back();

        if (m_vecChunks.empty())
        {
            m_allocChunk = NULL;
            m_deallocChunk = NULL;
        }
        else
        {
            if (m_deallocChunk == m_emptyChunk)
            {
                m_deallocChunk = &m_vecChunks.front();
                assert(m_deallocChunk->m_blocksAvailable < m_numBlocks);
            }
            if (m_allocChunk == m_emptyChunk)
            {
                m_allocChunk = &m_vecChunks.back();
                assert(m_allocChunk->m_blocksAvailable < m_numBlocks);
            }
        }

        m_emptyChunk = NULL;
        assert(0 == CountEmptyChunks());

        return true;
    }

    /** Releases unused spots from ChunkList.  This takes constant time
     with respect to # of Chunks, but actual time depends on underlying
     memory allocator.
     @return False if no unused spots, true if some found and released.
     */
    bool TrimChunkList(void)
    {
        if (m_vecChunks.empty())
        {
            assert(NULL == m_allocChunk);
            assert(NULL == m_deallocChunk);
        }

        if (m_vecChunks.size() == m_vecChunks.capacity())
            return false;
        // Use the "make-a-temp-and-swap" trick to remove excess capacity.
        VecChunk(m_vecChunks).swap(m_vecChunks);

        return true;
    }

    /** Returns count of empty Chunks held by this allocator.  Complexity
     is O(C) where C is the total number of Chunks - empty or used.
     */
    /**
     * @description: 返回emptyChunk的个数
     * @return {*}
     */
    std::size_t CountEmptyChunks(void) const
    {
#ifdef DO_EXTRA_LOKI_TESTS
        // This code is only used for specialized tests of the allocator.
        // It is #ifdef-ed so that its O(C) complexity does not overwhelm the
        // functions which call it.
        std::size_t count = 0;
        for (ChunkCIter it(m_vecChunks.begin()); it != m_vecChunks.end(); ++it)
        {
            const Chunk& chunk = *it;
            if (chunk.HasAvailable(m_numBlocks))
                ++count;
        }
        return count;
#else
        return (NULL == m_emptyChunk) ? 0 : 1;
#endif
    }

    /** Determines if FixedAllocator is corrupt.  Checks data members to
     see if any have erroneous values, or violate class invariants.  It
     also checks if any Chunk is corrupt.  Complexity is O(C) where C is
     the number of Chunks.  If any data is corrupt, this will return true
     in release mode, or assert in debug mode.
     */
    /**
     * @description: 是否有效
     * @return {*}
     */
    bool IsCorrupt(void) const
    {
        const bool isEmpty = m_vecChunks.empty();
        auto start(m_vecChunks.begin());
        auto last(m_vecChunks.end());
        const size_t emptyChunkCount = CountEmptyChunks();

        if (isEmpty)
        {
            if (start != last)
            {
                assert(false);
                return true;
            }
            if (0 < emptyChunkCount)
            {
                assert(false);
                return true;
            }
            if (NULL != m_deallocChunk)
            {
                assert(false);
                return true;
            }
            if (NULL != m_allocChunk)
            {
                assert(false);
                return true;
            }
            if (NULL != m_emptyChunk)
            {
                assert(false);
                return true;
            }
        }
        else
        {
            const Chunk* front = &m_vecChunks.front();
            const Chunk* back = &m_vecChunks.back();
            if (start >= last)
            {
                assert(false);
                return true;
            }
            if (back < m_deallocChunk)
            {
                assert(false);
                return true;
            }
            if (back < m_allocChunk)
            {
                assert(false);
                return true;
            }
            if (front > m_deallocChunk)
            {
                assert(false);
                return true;
            }
            if (front > m_allocChunk)
            {
                assert(false);
                return true;
            }

            switch (emptyChunkCount)
            {
            case 0:
                if ( m_emptyChunk != NULL)
                {
                    assert(false);
                    return true;
                }
                break;
            case 1:
                if ( m_emptyChunk == NULL)
                {
                    assert(false);
                    return true;
                }
                if (back < m_emptyChunk)
                {
                    assert(false);
                    return true;
                }
                if (front > m_emptyChunk)
                {
                    assert(false);
                    return true;
                }
                if (! m_emptyChunk->HasAvailable(m_numBlocks))
                {
                    // This may imply somebody tried to delete a block twice.
                    assert(false);
                    return true;
                }
                break;
            default:
                assert(false);
                return true;
            }
            for (auto it = start; it != last; ++it)
            {
                const Chunk& chunk = *it;
                if (chunk.IsCorrupt(m_numBlocks, m_blockSize, true))
                    return true;
            }
        }

        return false;
    }

    /** Returns true if the block at address p is within a Chunk owned by
     this FixedAllocator.  Complexity is O(C) where C is the total number
     of Chunks - empty or used.
     */
    const Chunk* HasBlock(void* p) const
    {
        for (auto it = m_vecChunks.begin(); it != m_vecChunks.end(); ++it)
        {
            const Chunk& chunk = *it;
            if (chunk.HasBlock(p))
                return &chunk;
        }
        return NULL;
    }
    inline Chunk* HasBlock(void* p)
    {
        return const_cast<Chunk*>(
            const_cast<const FixedAllocator*>(this)->HasBlock(p));
    }

    void MemInfo()
    {
        std::cout << "allocChunk:" << (m_allocChunk == nullptr ? "nullptr" : std::to_string(m_allocChunk - m_vecChunks.data())) << " "
                    << "deallocChunk:" << (m_deallocChunk == nullptr ? "nullptr" : std::to_string(m_deallocChunk - m_vecChunks.data())) << " "
                    << "emptyChunk:" << (m_emptyChunk == nullptr ? "nullptr" : std::to_string(m_emptyChunk - m_vecChunks.data())) << " "
                    << std::endl;
        for(std::size_t i = 0; i < m_vecChunks.size(); ++i)
        {
            std::cout << "\t" << i << "@";
            m_vecChunks[i].MemInfo();
        }
        
    }
private:
    /** Deallocates the block at address p, and then handles the internal
     bookkeeping needed to maintain class invariants.  This assumes that
     m_deallocChunk points to the correct chunk.
     */
    /**
     * @description: 释放p的内存块。默认认为p在m_deallocChunk中
     * @param {void*} p
     * @return {*}
     */
    void DoDeallocate(void* p)
    {
        // Show that m_deallocChunk really owns the block at address p.
        assert(m_deallocChunk->HasBlock(p));
        // Either of the next two assertions may fail if somebody tries to
        // delete the same block twice.
        assert( m_emptyChunk != m_deallocChunk);
        assert(!m_deallocChunk->HasAvailable(m_numBlocks));
        // prove either m_emptyChunk points nowhere, or points to a truly empty Chunk.
        assert((NULL == m_emptyChunk) || ( m_emptyChunk->HasAvailable(m_numBlocks)));

        // call into the chunk, will adjust the inner list but won't release memory
        m_deallocChunk->Deallocate(p);

        if (m_deallocChunk->HasAvailable(m_numBlocks))
        {
            assert( m_emptyChunk != m_deallocChunk);
            // m_deallocChunk is empty, but a Chunk is only released if there are 2
            // empty chunks.  Since m_emptyChunk may only point to a previously
            // cleared Chunk, if it points to something else besides m_deallocChunk,
            // then FixedAllocator currently has 2 empty Chunks.
            if (NULL != m_emptyChunk)
            {
                // If last Chunk is empty, just change what m_deallocChunk
                // points to, and release the last.  Otherwise, swap an empty
                // Chunk with the last, and then release it.
                Chunk* lastChunk = &m_vecChunks.back();
                if (lastChunk == m_deallocChunk)
                    m_deallocChunk = m_emptyChunk;
                else if (lastChunk != m_emptyChunk)
                    std::swap(* m_emptyChunk, *lastChunk);
                assert(lastChunk->HasAvailable(m_numBlocks));
                lastChunk->Release();
                m_vecChunks.pop_back();
                if ((m_allocChunk == lastChunk) || m_allocChunk->IsFilled())
                    m_allocChunk = m_deallocChunk;
            }
            m_emptyChunk = m_deallocChunk;
        }

        // prove either m_emptyChunk points nowhere, or points to a truly empty Chunk.
        assert((NULL == m_emptyChunk) || ( m_emptyChunk->HasAvailable(m_numBlocks)));
    }

    /** Creates an empty Chunk and adds it to the end of the ChunkList.
     All calls to the lower-level memory allocation functions occur inside
     this function, and so the only try-catch block is inside here.
     @return true for success, false for failure.
     */
    /**
     * @description: 创建一个Chunk
     * @return {*}
     */
    bool MakeNewChunk(void)
    {
        bool allocated = false;
        try
        {
            std::size_t size = m_vecChunks.size();
            // Calling m_vecChunks.reserve *before* creating and initializing the new
            // Chunk means that nothing is leaked by this function in case an
            // exception is thrown from reserve.
            if (m_vecChunks.capacity() == size)
            {
                if (0 == size)
                    size = 4;
                m_vecChunks.reserve(size * 2);
            }
            Chunk newChunk;
            allocated = newChunk.Init(m_blockSize, m_numBlocks);
            if (allocated)
            {
                m_vecChunks.push_back(newChunk);
            }
        }
        catch (...)
        {
            allocated = false;
        }
        if (!allocated)
            return false;

        m_allocChunk = &m_vecChunks.back();
        m_deallocChunk = &m_vecChunks.front();
        return true;
    }

    /** Finds the Chunk which owns the block at address p.  It starts at
     m_deallocChunk and searches in both forwards and backwards directions
     from there until it finds the Chunk which owns p.  This algorithm
     should find the Chunk quickly if it is m_deallocChunk or is close to it
     in the Chunks container.  This goes both forwards and backwards since
     that works well for both same-order and opposite-order deallocations.
     (Same-order = objects are deallocated in the same order in which they
     were allocated.  Opposite order = objects are deallocated in a last to
     first order.  Complexity is O(C) where C is count of all Chunks.  This
     never throws.
     @return 指向拥有p的Chunk的指针，如果没有找到所有者，返回nullptr
     */
    Chunk* VicinityFind(void* p) const
    {
        if (m_vecChunks.empty())
            return nullptr;

        assert(m_deallocChunk);

        const std::size_t chunkLength = m_numBlocks * m_blockSize;
        Chunk* lo = m_deallocChunk;
        Chunk* hi = m_deallocChunk + 1;
        const Chunk* loBound = &m_vecChunks.front();
        const Chunk* hiBound = &m_vecChunks.back() + 1;

        // Special case: m_deallocChunk is the last in the array
        if (hi == hiBound)
            hi = nullptr;

        for (;;)
        {
            if (lo)
            {
                if (lo->HasBlock(p))
                    return lo;
                if (lo == loBound)
                {
                    lo = nullptr;
                    if (nullptr == hi)
                        break;
                }
                else
                    --lo;
            }

            if (hi)
            {
                if (hi->HasBlock(p))
                    return hi;
                if (++hi == hiBound)
                {
                    hi = nullptr;
                    if (nullptr == lo)
                        break;
                }
            }
        }

        return nullptr;
    }

private:
    static uint8_t s_MinChunkBlockCount;    // Chunk中最小的内存块个数 [zhuyb 2023-12-06 17:16:14]
    static uint8_t s_MaxChunkBlockCount;    // Chunk中最大的内存块个数 [zhuyb 2023-12-06 17:16:34]

    /// Number of bytes in a single block within a Chunk.
    std::size_t m_blockSize = 0;
    /// Number of blocks managed by each Chunk.
    uint8_t m_numBlocks = 0;

    VecChunk m_vecChunks; // 所有的快容器 [zhuyb 2023-12-05 15:28:56]
    /// Pointer to Chunk used for last or next allocation.
    Chunk* m_allocChunk = nullptr;   // 上一次使用的chunk快 [zhuyb 2023-12-05 15:29:28]
    /// Pointer to Chunk used for last or next deallocation.
    Chunk* m_deallocChunk = nullptr;
    /// Pointer to the only empty Chunk if there is one, else NULL.
    Chunk* m_emptyChunk = nullptr;   // 指向还未使用的Chunk [zhuyb 2023-12-05 15:30:47]
};

uint8_t FixedAllocator::s_MinChunkBlockCount = 4;
uint8_t FixedAllocator::s_MaxChunkBlockCount = UINT8_MAX;
}
}