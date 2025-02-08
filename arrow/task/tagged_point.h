#pragma once
#include <cstdint>

namespace Arrow
{

namespace Pattern
{

// 标签指针 防止指针的ABA问题 [zhuyb 2023-10-15 17:24:01]
template <typename T>
struct TaggedPtr
{
    uint32_t u32Tag = 0;
    T* pNode = nullptr;

    TaggedPtr() noexcept : u32Tag(0), pNode(nullptr) {}

    TaggedPtr(const TaggedPtr& val) = default;

    explicit TaggedPtr(T* p, uint32_t tag = 0) : pNode(p), u32Tag(tag) {}

    TaggedPtr& operator=(const TaggedPtr& val) = default;

    bool operator==(volatile const TaggedPtr& val) const
    {
        return pNode == val.pNode && u32Tag == val.u32Tag;
    }

    bool operator!=(volatile const TaggedPtr& val) const
    {
        return !operator==(val);
    }
};
}
}