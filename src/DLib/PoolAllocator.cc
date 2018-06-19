#include "PoolAllocator.hh"
#include "Math.hh"

namespace dollop {


PoolAllocator::PoolAllocator( std::size_t objectSize, u8 objectAlignment,
                              std::size_t size, void* start )
        : Allocator( size, start ),
          m_objectSize( objectSize ),
          m_objectAlignment( objectAlignment ) {

    DLP_ASSERT( m_objectSize >= sizeof( void* ) );

    // Align memory
    u8 adjustment = pointer_math::alignForwardAdjustment( start, m_objectAlignment );
    m_freeList = static_cast< void** >( pointer_math::add( start, adjustment ) );

    // Amount of objects we can fit in our pool
    std::size_t numObjects = ( size - adjustment ) / m_objectSize;

    // Get pointer to memory start
    void** ptr = m_freeList;

    // I... don't understand what this is doing.
    for ( std::size_t i = 0; i < numObjects - 1; ++i ) {
        *ptr = pointer_math::add( ptr, m_objectSize );
        ptr = static_cast< void** >( *ptr );
    }

    *ptr = nullptr;
}


PoolAllocator::~PoolAllocator() {
    m_freeList = nullptr;
}

void* PoolAllocator::allocate( std::size_t size, u8 alignment ) {

    DLP_ASSERT( size == m_objectSize && alignment == m_objectAlignment );

    if ( m_freeList == nullptr ) {
        return nullptr;
    }

    void* ptr = m_freeList;
    m_freeList = static_cast< void** >( *m_freeList );

    m_usedMemory += size;
    m_numAllocations++;

    return ptr;
}


void PoolAllocator::deallocate( void* ptr ) {
    *( static_cast< void** >( ptr ) ) = m_freeList;
    m_freeList = static_cast< void** >( ptr );
    m_usedMemory -= m_objectSize;
    m_numAllocations--;
}

}
