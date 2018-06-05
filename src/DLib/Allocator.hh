#ifndef DOLLOP_ALLOCATOR_HH
#define DOLLOP_ALLOCATOR_HH

#include "Assert.hh"
#include "Log.hh"
#include "Platform.hh"

#include <cstddef>

namespace dollop {

class Allocator {

public:

    Allocator( std::size_t size, void *start );
    virtual ~Allocator();

    virtual void *allocate( std::size_t size, std::size_t alignment = 4 ) = 0;
    virtual void deallocate( void *ptr ) = 0;
    void *getStart() const { return m_start; }
    std::size_t getSize() const { return m_size; }
    std::size_t getUsedMemory() const { return m_usedMemory; }
    std::size_t getNumAllocations() const { return m_numAllocations; }

protected:

    void *m_start;
    std::size_t m_size;
    std::size_t m_usedMemory;
    std::size_t m_numAllocations;

};

namespace allocator {

template< typename T >
T* allocateNew( Allocator& allocator ) {
    return new ( allocator.allocate( sizeof( T ), alignof( T ) ) )T;
}

template< typename T >
T *allocateNew( Allocator& allocator, const T& t ) {
    return new ( allocator.allocate( sizeof( T ), alignof( T ) ) )T( t );
}

template< typename T >
T *deallocateNew( Allocator& allocator, T& object ) {
    object.~T();
    allocator.deallocate( &object );
}

template< typename T >
T *allocateArray( Allocator& allocator, std::size_t length ) {
    DLP_ASSERT( length > 0 );
    std::size_t headerSize = sizeof( std::size_t ) / sizeof( T );
    if ( sizeof( std::size_t ) % sizeof( T ) > 0 ) {
        headerSize += 1;
    }

    // Allocate extra space to store array length in the bytes before the array
    T* ptr = static_cast< T* >( allocator.allocate( sizeof( T ) * ( length + headerSize ), alignof( T ) ) ) + headerSize;
    *( static_cast< std::size_t* >( ptr ) - 1 ) = length;
    for ( std::size_t i = 0; i < length; ++i ) {
        new ( &ptr )T;
    }

    return ptr;
}

template< typename T >
void deallocateArray( Allocator& allocator, T *array ) {
    DLP_ASSERT( array != nullptr );
    std::size_t length = *( ( ( std::size_t * )array ) - 1 );
    for ( std::size_t i = 0; i < length; ++i ) {
        array[i].~T();
    }

    // Calculate how much extra memory was allocated to store the length before the array
    std::size_t headerSize = sizeof( std::size_t ) / sizeof( T );
    if ( sizeof( std::size_t ) % sizeof( T ) > 0 ) {
        headerSize += 1;
    }
    allocator.deallocate( array - headerSize );
}

} // namespace allocator

} // namespace dollop

#endif // DOLLOP_ALLOCATOR_HH
