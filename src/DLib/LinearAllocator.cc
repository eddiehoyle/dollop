#include "LinearAllocator.hh"
#include "Math.hh"

namespace dollop {

LinearAllocator::LinearAllocator( std::size_t size, void *start )
        : Allocator( size, start ),
          m_currentPos( start ) {
    DLP_ASSERT( size > 0 );
}

LinearAllocator::~LinearAllocator() {
    m_currentPos = nullptr;
}

void *LinearAllocator::allocate( std::size_t size, u8 alignment ) {
    DLP_ASSERT( size > 0 );
    u8 adjustment = pointer_math::alignForwardAdjustment( m_currentPos, alignment );

    if ( m_usedMemory + adjustment + size > m_size ) {
        return nullptr;
    }

    uptr aligned_address = reinterpret_cast< uptr >( m_currentPos ) + adjustment;
    m_currentPos = reinterpret_cast< void* >( aligned_address + size );
    m_usedMemory += size + adjustment;
    m_numAllocations++;

    return reinterpret_cast< void* >( aligned_address );
}

void LinearAllocator::deallocate( void *ptr ) {
    DLP_ASSERT( false && "use clear() instead" );
}

void* LinearAllocator::head() const {
    return m_currentPos;
}

void LinearAllocator::clear() {
    m_numAllocations = 0;
    m_usedMemory = 0;
    m_currentPos = m_start;
}

}