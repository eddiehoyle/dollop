#include <iostream>

#include <DLib/FreeListAllocator.hh>
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

    FreeListAllocator *allocator = new( memory )FreeListAllocator( memory_size - sizeof( FreeListAllocator ),
                                                         pointer_math::add( memory, sizeof( FreeListAllocator ) ) );
    DLP_ASSERT( allocator != nullptr );

    Mesh *meshA = createMesh( *allocator );
    Mesh *meshB = createMesh( *allocator );
    Mesh *meshC = createMesh( *allocator );
    DLP_ASSERT( meshA != nullptr );
    DLP_ASSERT( meshB != nullptr );
    DLP_ASSERT( meshC != nullptr );
    DLP_ERROR << (void*)meshA;
    DLP_ERROR << (void*)meshB;
    DLP_ERROR << (void*)meshC;

    free( memory );

    return 0;
}