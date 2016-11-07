#include "WCLinearAlloc.hpp"

#include "WCAssert.hpp"

WC_THIRD_PARTY_INCLUDES_BEGIN
    #include <cstdlib>
WC_THIRD_PARTY_INCLUDES_END

WC_BEGIN_NAMESPACE

LinearAlloc::LinearAlloc(size_t maxAllocSize,
                         size_t blockSize,
                         size_t reserveNumBlocks,
                         size_t reserveNumSysBlocks)
:
    mMaxAllocSize(maxAllocSize),
    mBlockSize(blockSize)
{
    // Sanity checks: max alloc size and block size must be at least this size
    WC_ASSERT(maxAllocSize >= kAlign * 2);
    WC_ASSERT(blockSize >= kAlign * 2);
    
    // Sanity checks: block size must be at least same size as max alloc size
    WC_ASSERT(blockSize >= maxAllocSize);
    
    // Reserving:
    mMemBlocks.reserve(reserveNumBlocks);
    mSysMemBlocks.reserve(reserveNumSysBlocks);
    
    // Make sure there is always at least one mem block
    allocMemBlock();
}

LinearAlloc::~LinearAlloc() {
    for (const MemBlock & block : mMemBlocks) {
        std::free(block.startPtr);
    }
    
    for (const SysMemBlock & block : mSysMemBlocks) {
        std::free(block.startPtr);
    }
}

void * LinearAlloc::alloc(size_t numBytes) {
    // Zero sized allocs do nothing
    if (numBytes == 0) {
        return nullptr;
    }
    
    // If the alloc is too big then do a system alloc instead
    if (numBytes > mMaxAllocSize) {
        return sysAlloc(numBytes);
    }
    
    // See if we have enough room in the currently allocated block to satisfy the alloc:
    {
        MemBlock & lastBlock = mMemBlocks.back();
        
        if (lastBlock.currentOffset + numBytes <= mBlockSize) {
            // Happy days- we can service this alloc: compute the return pointer, move along in the block
            // and give back the mem...
            void * returnPtr = reinterpret_cast<int8_t*>(lastBlock.startPtr) + lastBlock.currentOffset;
            lastBlock.currentOffset = alignBlockOffset(lastBlock.currentOffset + numBytes);
            return returnPtr;
        }
    }
    
    // Can't service this request with the currently allocated block, make a new one!
    // Move along the current block offset then and return the start block pointer.
    allocMemBlock();
    MemBlock & lastBlock = mMemBlocks.back();
    lastBlock.currentOffset = alignBlockOffset(numBytes);
    return lastBlock.startPtr;
}

void LinearAlloc::allocMemBlock() {
    mMemBlocks.resize(mMemBlocks.size() + 1);
    MemBlock & block = mMemBlocks.back();
    block.startPtr = std::malloc(mBlockSize);
    
    if (!block.startPtr) {
        std::exit(-1);   // Out of memory!
    }
    
    block.currentOffset = 0;
}

void * LinearAlloc::sysAlloc(size_t numBytes) {
    return std::malloc(numBytes);
}

size_t LinearAlloc::alignBlockOffset(size_t offset) {
    size_t remainder = offset % kAlign;
    
    if (remainder > 0) {
        offset += kAlign - remainder;
    }
    
    return offset;
}

WC_END_NAMESPACE
