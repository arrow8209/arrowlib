#pragma once
#include <stdint.h>
#include <cstdlib>
#include <assert.h>
#include <bitset>
#include <cstdint>
#include <iostream>

namespace Arrow2
{
namespace Other
{
class Chunk
{
public:
    // friend class FixedAllocator;

    /*
    说明：
    1.Chunk中最大的可分配255个内存块
    2.每个内存块的第一字节记录，下个内存的索引号
    3.有一个变量m_firstAvailableBlock 当前还未使用的索引号
    4.每分配一个内存块m_firstAvailableBlock 指向新的索引号
    5.回收内存，根据地址偏移量计算索引号，并记录在m_firstAvailableBlock中
    */
    Chunk(){}
    ~Chunk()
    {
        // if (m_pData == nullptr)
        //     return;
        // Release();
    }
    /**
     * @description: 初始化内存
     * @param {size_t} blockSize 每个内存块的字节数
     * @param {uint8_t} blockCount 内存块个数
     * @return {*}
     */
    bool Init(std::size_t blockSize, uint8_t blockCount)
    {
        m_blockSize = blockSize;
        m_blockCount = blockCount;
        const std::size_t allocSize = blockSize * blockCount;
        m_pData = static_cast<uint8_t*>(std::malloc(allocSize));
        Reset(blockSize, blockCount);
        return true;
    }

    /**
     * @description: 分配内存
     * @param {size_t} blockSize
     * @return {*}
     */
    void* Allocate()
    {
        if (IsFilled())
            return NULL;

        uint8_t* pResult = m_pData + (m_firstAvailableBlock * m_blockSize);
        m_firstAvailableBlock = *pResult;
        --m_blocksAvailable;

        return pResult;
    }

    /**
     * @description: 回收内存
     * @param {void*} p
     * @param {size_t} blockSize
     * @return {*}
     */
    void Deallocate(void* p)
    {
        assert(p >= m_pData);

        uint8_t* toRelease = static_cast<uint8_t*>(p);
        // Alignment check
        assert((toRelease - m_pData) % m_blockSize == 0);
        uint8_t index = static_cast<uint8_t>((toRelease - m_pData) / m_blockSize);

#if defined(DEBUG) || defined(_DEBUG)
        // Check if block was already deleted.  Attempting to delete the same
        // block more than once causes Chunk's linked-list of stealth indexes to
        // become corrupt.  And causes count of blocksAvailable_ to be wrong.
        if (0 < blocksAvailable_)
            assert(m_firstAvailableBlock != index);
#endif

        *toRelease = m_firstAvailableBlock;
        m_firstAvailableBlock = index;
        // Truncation check
        assert(m_firstAvailableBlock == (toRelease - m_pData) / m_blockSize);

        ++m_blocksAvailable;
    }


    /**
     * @description: 初始化内存
     * 每个块的第一个字节指向下一个内存快的索引号
     * @param {size_t} blockSize 每个块的大小
     * @param {uint8_t} blockCount 内存块的个数
     * @return {*}
     */
    void Reset(std::size_t blockSize, uint8_t blockCount)
    {
        assert(blockSize > 0);
        assert(blockCount > 0);
        // Overflow check
        assert((blockSize * blockCount) / blockSize == blockCount);

        m_firstAvailableBlock = 0;
        m_blocksAvailable = blockCount;

        uint8_t i = 0;
        for (uint8_t* p = m_pData; i != blockCount; p += blockSize)
        {
            *p = ++i;
        }
    }

    /**
     * @description: 释放内存
     * @return {*}
     */
    void Release()
    {
        ::std::free(static_cast<void*>(m_pData));
        m_pData = nullptr;
        m_firstAvailableBlock = 0;
        m_blocksAvailable = 0;
        m_blockSize = 0;
        m_blockCount = 0;
    }

    /** Determines if the Chunk has been corrupted.
     @param numBlocks Total # of blocks in the Chunk.
     @param blockSize # of bytes in each block.
     @param checkIndexes True if caller wants to check indexes of available
      blocks for corruption.  If false, then caller wants to skip some
      tests tests just to run faster.  (Debug version does more checks, but
      release version runs faster.)
     @return True if Chunk is corrupt.
     */
    bool IsCorrupt(uint8_t numBlocks, std::size_t blockSize, bool checkIndexes) const
    {
        if (numBlocks < m_blocksAvailable)
        {
            // Contents at this Chunk corrupted.  This might mean something has
            // overwritten memory owned by the Chunks container.
            assert(false);
            return true;
        }
        if (IsFilled())
            // Useless to do further corruption checks if all blocks allocated.
            return false;
        uint8_t index = m_firstAvailableBlock;
        if (numBlocks <= index)
        {
            // Contents at this Chunk corrupted.  This might mean something has
            // overwritten memory owned by the Chunks container.
            assert(false);
            return true;
        }
        if (!checkIndexes)
            // Caller chose to skip more complex corruption tests.
            return false;

        /* If the bit at index was set in foundBlocks, then the stealth index was
         found on the linked-list.
         */
        std::bitset<UINT8_MAX> foundBlocks;
        uint8_t* nextBlock = NULL;

        /* The loop goes along singly linked-list of stealth indexes and makes sure
         that each index is within bounds (0 <= index < numBlocks) and that the
         index was not already found while traversing the linked-list.  The linked-
         list should have exactly blocksAvailable_ nodes, so the for loop will not
         check more than blocksAvailable_.  This loop can't check inside allocated
         blocks for corruption since such blocks are not within the linked-list.
         Contents of allocated blocks are not changed by Chunk.

         Here are the types of corrupted link-lists which can be verified.  The
         corrupt index is shown with asterisks in each example.

         Type 1: Index is too big.
          numBlocks == 64
          blocksAvailable_ == 7
          m_firstAvailableBlock -> 17 -> 29 -> *101*
          There should be no indexes which are equal to or larger than the total
          number of blocks.  Such an index would refer to a block beyond the
          Chunk's allocated domain.

         Type 2: Index is repeated.
          numBlocks == 64
          blocksAvailable_ == 5
          m_firstAvailableBlock -> 17 -> 29 -> 53 -> *17* -> 29 -> 53 ...
          No index should be repeated within the linked-list since that would
          indicate the presence of a loop in the linked-list.
         */
        for (uint8_t cc = 0;;)
        {
            nextBlock = m_pData + (index * blockSize);
            foundBlocks.set(index, true);
            ++cc;
            if (cc >= m_blocksAvailable)
                // Successfully counted off number of nodes in linked-list.
                break;
            index = *nextBlock;
            if (numBlocks <= index)
            {
                /* This catches Type 1 corruptions as shown in above comments.
                 This implies that a block was corrupted due to a stray pointer
                 or an operation on a nearby block overran the size of the block.
                 */
                assert(false);
                return true;
            }
            if (foundBlocks.test(index))
            {
                /* This catches Type 2 corruptions as shown in above comments.
                 This implies that a block was corrupted due to a stray pointer
                 or an operation on a nearby block overran the size of the block.
                 Or perhaps the program tried to delete a block more than once.
                 */
                assert(false);
                return true;
            }
        }
        if (foundBlocks.count() != m_blocksAvailable)
        {
            /* This implies that the singly-linked-list of stealth indexes was
             corrupted.  Ideally, this should have been detected within the loop.
             */
            assert(false);
            return true;
        }

        return false;
    }

    /**
     * @description: p 是否已经被可用
     * @param {void*} p
     * @return {*} true:可用 false:不可用
     */
    bool IsBlockAvailable(void* p) const
    {
        if (IsFilled())
            return false;

        uint8_t* place = static_cast<uint8_t*>(p);
        // Alignment check
        assert((place - m_pData) % m_blockSize == 0);
        uint8_t blockIndex = static_cast<uint8_t>((place - m_pData) / m_blockSize);

        uint8_t index = m_firstAvailableBlock;
        if (index == blockIndex)
            return true;

        /* If the bit at index was set in foundBlocks, then the stealth index was
         found on the linked-list.
         */
        std::bitset<UINT8_MAX> foundBlocks;
        uint8_t* nextBlock = NULL;
        for (uint8_t cc = 0;;)
        {
            nextBlock = m_pData + (index * m_blockSize);
            foundBlocks.set(index, true);
            ++cc;
            if (cc >= m_blocksAvailable)
                // Successfully counted off number of nodes in linked-list.
                break;
            index = *nextBlock;
            if (index == blockIndex)
                return true;
            assert(!foundBlocks.test(index));
        }

        return false;
    }

    /**
     * @description: 判断 p 是否在chunk 中
     * @param {void*} p 需要判断的指针
     * @param {size_t} chunkLength 指针的长度
     * @return {*} true p在chunk中
     */
    inline bool HasBlock(void* p) const
    {
        uint8_t* pc = static_cast<uint8_t*>(p);
        return (m_pData <= pc) && (pc < m_pData + m_blockSize * m_blockCount);
    }

    /**
     * @description: 判读可用块是否和numblocks一样
     * @param {uint8_t} numBlocks
     * @return {*}
     */
    inline bool HasAvailable(uint8_t numBlocks) const
    {
        return (m_blocksAvailable == numBlocks);
    }

    /**
     * @description: 是否满判断
     * @return {*} true 内存用完了 false 还有空闲
     */
    inline bool IsFilled(void) const
    {
        return (0 == m_blocksAvailable);
    }

    void MemInfo()
    {
        std::bitset<UINT8_MAX> foundBlocks;
        uint8_t u8Tmp = m_firstAvailableBlock;
        foundBlocks.set(u8Tmp);
        
        for(int i = 0; i < m_blocksAvailable; ++i)
        {
            u8Tmp = *(m_pData + u8Tmp * m_blockSize);
            foundBlocks.set(u8Tmp);
        }

        std::cout << static_cast<int32_t>(m_firstAvailableBlock) << ":" << static_cast<int32_t>(m_blocksAvailable) << "@";
        for(int i = 0; i < m_blockCount; ++i)
        {
            if(!foundBlocks.test(i))
            {
                std::cout << "[" << i << ":N]";
            }
            else
            {
                std::cout << "[" << i << ":" << static_cast<int32_t>(*(m_pData + i * m_blockSize))  << "]";
            }
        }

        if(m_blocksAvailable == 0)
        {
            std::cout << "empty" << std::endl;
        }
        else
        {
            std::cout << std::endl;
        }
        
    }

    uint8_t* m_pData = nullptr;        // 内存块 [zhuyb 2023-12-05 14:32:29]
    uint8_t m_firstAvailableBlock = 0; // 第一个空快的所以 [zhuyb 2023-12-05 14:32:47]
    uint8_t m_blocksAvailable = 0;     // 剩余未使用的内存块个数 [zhuyb 2023-12-05 14:33:06]
    std::size_t m_blockSize = 0;
    uint8_t m_blockCount = 0;
};
}
}