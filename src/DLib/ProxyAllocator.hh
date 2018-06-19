#ifndef DOLLOP_PROXYALLOCATOR_HH
#define DOLLOP_PROXYALLOCATOR_HH

#include "Allocator.hh"

namespace dollop {

class ProxyAllocator : public Allocator {

public:

    ProxyAllocator( Allocator& allocator );
    ~ProxyAllocator() = default;

    void* allocate( std::size_t size, u8 alignment ) override;
    void deallocate( void* ptr ) override;

private:

    ProxyAllocator( const ProxyAllocator& );
    ProxyAllocator& operator=( const ProxyAllocator& );

private:

    Allocator& m_allocator;

};

}

#endif // DOLLOP_PROXYALLOCATOR_HH
