#ifndef DOLLOP_POOLALLOCATOR_HH
#define DOLLOP_POOLALLOCATOR_HH

#include "Allocator.hh"

namespace dollop {

template< typename T >
class PoolAllocator : public Allocator {

public:

    PoolAllocator( std::size_t size, void* start );
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
