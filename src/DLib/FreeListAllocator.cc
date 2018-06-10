#include "FreeListAllocator.hh"
#include "Math.hh"

namespace dollop {

FreeListAllocator::FreeListAllocator( std::size_t size, void* start )
        : Allocator( size, start ),
          m_freeBlocks( static_cast< FreeBlock* >( start ) ) {
    DLP_ASSERT( size > sizeof( FreeBlock ) );
    m_freeBlocks->size = size;
    m_freeBlocks->next = nullptr;
}

FreeListAllocator::~FreeListAllocator() {
    m_freeBlocks = nullptr;
}

void* FreeListAllocator::allocate( std::size_t size, u8 alignment ) {
    DLP_ASSERT( size > 0 && alignment > 0 );
    FreeBlock* previousFreeBlock = nullptr;
    FreeBlock* freeBlock = m_freeBlocks;

    while( freeBlock != nullptr ) {
        u8 adjustment = pointer_math::alignForwardAdjustmentWithHeader< AllocationHeader >( freeBlock, alignment );
        std::size_t totalSize = size + adjustment;

        // If allocation doesn't fit in this FreeBlock, try the next
        if ( freeBlock->size < totalSize ) {
            previousFreeBlock = freeBlock;
            freeBlock = freeBlock->next;
            continue;
        }

        DLP_ASSERT( sizeof( AllocationHeader ) >= sizeof( FreeBlock ) );

        if ( freeBlock->size - totalSize  <= sizeof( AllocationHeader ) ){

            // Increase allocation size instead of creating a new FreeBlock
            totalSize = freeBlock->size;

            if ( previousFreeBlock != nullptr ) {
                previousFreeBlock = freeBlock->next;
            } else {
                m_freeBlocks = freeBlock->next;
            }
        } else {

            // Else create a new FreeBlock containing remaining memory
            FreeBlock* nextBlock = static_cast< FreeBlock* >( pointer_math::add( freeBlock, totalSize ) );
            nextBlock->size = freeBlock->size - totalSize;
            nextBlock->next = freeBlock->next;

            if ( previousFreeBlock != nullptr ) {
                previousFreeBlock->next = nextBlock;
            } else {
                m_freeBlocks = nextBlock;
            }
        }

        uptr alignedAddress = reinterpret_cast< uptr >( freeBlock ) + adjustment;
        AllocationHeader* header = reinterpret_cast< AllocationHeader* >( alignedAddress - sizeof( AllocationHeader ) );
        header->size = totalSize;
        header->adjustment = adjustment;
        m_usedMemory += totalSize;
        m_numAllocations++;

        DLP_ASSERT( pointer_math::alignForwardAdjustment( (void*)alignedAddress, alignment) == 0 );

        return (void*)alignedAddress;
    }
}

void FreeListAllocator::deallocate( void* ptr ) {

}

}