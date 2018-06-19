#ifndef DOLLOP_POOLALLOCATOR_HH
#define DOLLOP_POOLALLOCATOR_HH

#include "Allocator.hh"

namespace dollop {

/// This allocator only allows allocations of a fixed size and alignment to be
/// made, this results in both fast allocations and deallocations to be made.
/// Like the FreeList allocator, a linked-list of free blocks is maintaied
/// but since all blocks are the same size each free block only needs to
/// store a pointer to the next one. Another advantage of Pool allactors is
/// no need to align each allocation, since all allocations have the same
/// size/alignment only the first block has to be aligned, this results in a
/// almost non-existant memory overhead.
class PoolAllocator : public Allocator {

public:

    PoolAllocator( std::size_t objectSize, u8 objectAlignment,
                   std::size_t size, void* start );
    ~PoolAllocator();

    void* allocate( std::size_t size, u8 alignment ) override;
    void deallocate( void* ptr ) override;

private:

    PoolAllocator( const PoolAllocator& );
    PoolAllocator& operator=( const PoolAllocator& );

private:

    u8 m_objectAlignment;
    std::size_t m_objectSize;
    void** m_freeList;

};

}

#endif // DOLLOP_POOLALLOCATOR_HH
