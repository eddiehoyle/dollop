#include "Allocator.hh"

namespace dollop {

Allocator::Allocator( std::size_t size, void *start )
        : m_size( size ),
          m_start( start ),
          m_usedMemory( 0 ),
          m_numAllocations( 0 ) {}

Allocator::~Allocator() {
    DLP_ASSERT( m_numAllocations == 0 && m_usedMemory == 0 );
    m_start = nullptr;
    m_size = 0;
}

}
