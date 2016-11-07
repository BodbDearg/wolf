#pragma once

#include "WCMacros.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <vector>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

/**
 * A linear allocator used by the compiler/lexer to improve performance. Has the following properties:
 *
 * 1 - Memory is preallocated in large chunks and then dished out in a linear fashion.
 * 2 - Freeing memory is not supported. The allocator is designed for usage patterns where this 
 *     limitation is generally not a problem.
 * 3 - When the allocator is destroyed all memory allocated by this allocator is freed.
 * 4 - All memory allocated is aligned along sizeof(void*) bounds.
 */
class LinearAlloc {
public:
    /* The alignment for the allocator. All allocs are aligned on this num-bytes boundary. */
    static constexpr const size_t kAlign = sizeof(void*);
    
    /* Create the allocator with the given settings */
    LinearAlloc(size_t maxAllocSize,
                size_t blockSize,
                size_t reserveNumBlocks = 0,
                size_t reserveNumSysBlocks = 0);
    
    ~LinearAlloc();
    
    /* Allocate a chunk of memory */
    void * alloc(size_t numBytes);
    
    /* Allocate the the size of a given type */
    template <typename T>
    void * allocSizeOf() {
        return alloc(sizeof(T));
    }
    
private:
    /* A memory block preallocated in the linear allocator */
    struct MemBlock {
        void * startPtr;
        size_t currentOffset;
    };
    
    /* Type for a block allocated with the system allocator */
    struct SysMemBlock {
        void * startPtr;
    };
    
    /* Allocates a memory block */
    void allocMemBlock();
    
    /* Allocate a chunk of memory from the system and return it's pointer. The chunk is saved as a sys mem block. */
    void * sysAlloc(size_t numBytes);
    
    /* Utility, align the given block offset */
    size_t alignBlockOffset(size_t offset);
    
    /* Max alloc size */
    size_t mMaxAllocSize;
    
    /* Block size */
    size_t mBlockSize;
    
    /* The list of memory blocks allocated */
    std::vector<MemBlock> mMemBlocks;
    
    /* The list of system memory blocks allocated */
    std::vector<SysMemBlock> mSysMemBlocks;
};

WC_END_NAMESPACE
