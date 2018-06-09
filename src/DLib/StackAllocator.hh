#ifndef DOLLOP_STACKALLOCATOR_HH
#define DOLLOP_STACKALLOCATOR_HH

#include "Allocator.hh"

namespace dollop {

class StackAllocator : public Allocator {

public:

    StackAllocator( std::size_t size, void* start );
    ~StackAllocator();

    void* allocate( std::size_t size, u8 alignment ) override;
    void deallocate( void* ptr ) override;
    void clear();
    void* top() const;

private:

    StackAllocator( const StackAllocator& );
    StackAllocator& operator=( const StackAllocator& );

private:

    struct AllocationHeader {
        void* previous;
        u8 adjustment;
    };

    void* m_currentPos;
    void* m_previousPos;

};

}

#endif // DOLLOP_STACKALLOCATOR_HH
