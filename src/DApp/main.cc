#include <iostream>

#include <DLib/LinearAllocator.hh>
#include <DLib/Math.hh>

using namespace dollop;

struct Mesh {
    std::size_t vertices;
};

inline Mesh *createMesh( Allocator& allocator ) {
    return reinterpret_cast< Mesh * >( allocator.allocate( sizeof( Mesh ), alignof( Mesh ) ) );
}

int main( int argc, char **argv ) {

    std::size_t memory_size = 1024ULL * 1024;
    void *memory = malloc( memory_size );
    DLP_ASSERT( memory != nullptr );

    DLP_ERROR << "memory_size=" << memory_size << ", linear=" << sizeof( LinearAllocator );

    LinearAllocator *allocator = new( memory )LinearAllocator( memory_size - sizeof( LinearAllocator ),
                                                         pointer_math::add( memory, sizeof( LinearAllocator ) ) );
    DLP_ASSERT( allocator != nullptr );

    DLP_ERROR << allocator->head();
    Mesh *mesh = createMesh( *allocator );
    DLP_ASSERT( mesh != nullptr );
    DLP_ERROR << (void*)mesh;

    DLP_ERROR << allocator->head();
    Mesh *mesh2 = createMesh( *allocator );
    DLP_ASSERT( mesh2 != nullptr );
    DLP_ERROR << (void*)mesh2;

    return 0;
}