#ifndef DOLLOP_LINEARALLOCATOR_HH
#define DOLLOP_LINEARALLOCATOR_HH

#include "Allocator.hh"

namespace dollop {

class LinearAllocator : public Allocator {

public:

    LinearAllocator( std::size_t size, void* start );
    ~LinearAllocator();

    void* allocate( std::size_t size, u8 alignment ) override;
    void deallocate( void* ptr ) override;
    void clear();
    void* head() const;

private:

    LinearAllocator( const LinearAllocator& );
    LinearAllocator& operator=( const LinearAllocator& );

private:

    void* m_currentPos;

};

}


#endif //DOLLOP_LINEARALLOCATOR_HH
