#include "PoolAllocator.hh"
#include "Math.hh"

namespace dollop {

template< typename T >
PoolAllocator< T >::PoolAllocator( std::size_t size, void* start )
        : Allocator( size, start ),
          m_objectSize( sizeof( T ) ),
          m_objectAlignment( alignof( T ) ) {

    DLP_ASSERT( m_objectSize >= sizeof( void* ) );
    u8 adjustment = pointer_math::alignForwardAdjustment( start, m_objectAlignment );
    m_freeList = static_cast< void** >( pointer_math::add( start, adjustment ) );
    std::size_t numObjects = ( size - adjustment ) / m_objectSize;
    void** ptr = m_freeList;

    for ( std::size_t i = 0; i < numObjects - 1; ++i ) {
        *ptr = pointer_math::add( ptr, m_objectSize );
        ptr = static_cast< void** >( *ptr );
    }

    *ptr = nullptr;
}

template< typename T >
PoolAllocator< T >::~PoolAllocator() {

}

template< typename T >
void* PoolAllocator< T >::allocate( std::size_t size, u8 alignment ) {
    return nullptr;
}

template< typename T >
void PoolAllocator< T >::deallocate( void* ptr ) {

}

}
