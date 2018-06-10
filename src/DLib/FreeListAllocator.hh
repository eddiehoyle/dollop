#ifndef DOLLOP_FREELIST_HH
#define DOLLOP_FREELIST_HH

#include "Allocator.hh"

namespace dollop {

class FreeListAllocator : public Allocator {

public:

    FreeListAllocator( std::size_t size, void* start );
    ~FreeListAllocator();

    void* allocate( std::size_t size, u8 alignment ) override;
    void deallocate( void* ptr ) override;

private:

    struct AllocationHeader {
        std::size_t size;
        u8 adjustment;
    };

    struct FreeBlock {
        std::size_t size;
        FreeBlock* next;
    };

    FreeListAllocator( const FreeListAllocator& );
    FreeListAllocator& operator=( const FreeListAllocator& );

    FreeBlock* m_freeBlocks;
};

}

#endif // DOLLOP_FREELIST_HH
