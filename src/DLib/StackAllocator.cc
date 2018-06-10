#include "StackAllocator.hh"
#include "Math.hh"

namespace dollop {

StackAllocator::StackAllocator( std::size_t size, void* start )
        : Allocator( size, start ),
          m_currentPos( start ),
          m_previousPos( nullptr ) {
    DLP_ASSERT( size > 0 );
}

StackAllocator::~StackAllocator() {
    m_previousPos = nullptr;
    m_currentPos = nullptr;
}

void* StackAllocator::allocate( std::size_t size, u8 alignment ) {
    DLP_ASSERT( size > 0 );
    u8 adjustment = pointer_math::alignForwardAdjustmentWithHeader< AllocationHeader >( m_currentPos, alignment );
    if ( m_usedMemory + adjustment + size > m_size ) {
        return nullptr;
    }
    void* alignedAddress = pointer_math::add( m_currentPos, adjustment );

    AllocationHeader* header = static_cast< AllocationHeader* >(pointer_math::subtract(alignedAddress, sizeof(AllocationHeader)));
    header->adjustment = adjustment;
    header->previous = m_previousPos;
    m_previousPos = alignedAddress;

    m_currentPos = pointer_math::add( alignedAddress, size );
    m_usedMemory += size + adjustment;
    m_numAllocations++;

    return alignedAddress;
}

void StackAllocator::deallocate( void* ptr ) {
    DLP_ASSERT( ptr == m_previousPos );

    AllocationHeader* header = reinterpret_cast< AllocationHeader* >( pointer_math::subtract( ptr,
                                                                                              sizeof( AllocationHeader ) ) );
    m_usedMemory -= reinterpret_cast< uptr >( m_currentPos ) - reinterpret_cast< uptr >( ptr ) + header->adjustment;
    m_currentPos = pointer_math::subtract( ptr, header->adjustment );

    m_previousPos = header->previous;

    m_numAllocations--;
}

void* StackAllocator::top() const {
    return m_previousPos;
}

void StackAllocator::clear() {
    while ( m_numAllocations > 0 ) {
        void* ptr = m_currentPos;
        deallocate( ptr );
    }
}

}