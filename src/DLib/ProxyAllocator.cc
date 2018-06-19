#include "ProxyAllocator.hh"

namespace dollop {

ProxyAllocator::ProxyAllocator( Allocator& allocator )
    : Allocator( allocator.getSize(), allocator.getStart() ),
      m_allocator( allocator ) {
}

void* ProxyAllocator::allocate( std::size_t size, u8 alignment ) {
    DLP_ASSERT( size != 0 );
    m_numAllocations++;
    std::size_t mem = m_allocator.getUsedMemory();

    void* ptr = m_allocator.allocate( size, alignment );
    m_usedMemory += m_allocator.getUsedMemory() - mem;
    return ptr;
}

void ProxyAllocator::deallocate( void* ptr ) {
    m_numAllocations--;
    std::size_t mem = m_allocator.getUsedMemory();
    m_allocator.deallocate( ptr );
    m_usedMemory -= mem - m_allocator.getUsedMemory();
}

}