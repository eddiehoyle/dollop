#include "FreeListAllocator.hh"
#include "Math.hh"

namespace dollop {

FreeListAllocator::FreeListAllocator( std::size_t size, void* start )
        : Allocator( size, start ),
          m_freeBlocks( static_cast< FreeBlock* >( start ) ) {

    DLP_ASSERT( size > sizeof( FreeBlock ) );

    // First free block is the size of initial memory block
    m_freeBlocks->size = size;
    m_freeBlocks->next = nullptr;
}

FreeListAllocator::~FreeListAllocator() {
    m_freeBlocks = nullptr;
}

void* FreeListAllocator::allocate( std::size_t size, u8 alignment ) {

    DLP_ASSERT( size > 0 && alignment > 0 );

    // Get pointer to first free block
    FreeBlock* previousFreeBlock = nullptr;
    FreeBlock* freeBlock = m_freeBlocks;

    // Loop through all linked free blocks
    while( freeBlock != nullptr ) {

        // Size of header + data
        u8 adjustment = pointer_math::alignForwardAdjustmentWithHeader< AllocationHeader >( freeBlock, alignment );
        std::size_t totalSize = size + adjustment;

//        DLP_ERROR << "freeblock size=" << freeBlock->size << ", totalSize=" << totalSize;

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

    DLP_ASSERT( false && "Couldn't find free block large enough!" );
    return nullptr;
}

void FreeListAllocator::deallocate( void* ptr ) {
    DLP_ASSERT( ptr != nullptr );
    AllocationHeader* header = reinterpret_cast< AllocationHeader* >( pointer_math::subtract( ptr, sizeof( AllocationHeader ) ) );
    uptr block_start = reinterpret_cast< uptr >( ptr ) - header->adjustment;
    std::size_t block_size = header->size;
    uptr block_end = block_start + block_size;

    FreeBlock* previousFreeBlock = nullptr;
    FreeBlock* freeBlock = m_freeBlocks;

    while ( freeBlock != nullptr ) {
        if ( reinterpret_cast< uptr >( freeBlock ) >= block_end ) {
            break;
        }
        previousFreeBlock = freeBlock;
        freeBlock = freeBlock->next;
    }

    if ( previousFreeBlock == nullptr ) {
        previousFreeBlock = reinterpret_cast< FreeBlock* >( block_start );
        previousFreeBlock->size = block_size;
        previousFreeBlock->next = m_freeBlocks; // ???
        m_freeBlocks = previousFreeBlock; // ???
    } else if ( reinterpret_cast< uptr >( previousFreeBlock ) + previousFreeBlock->size == block_start ) {
        previousFreeBlock->size += block_size;
    } else {
        FreeBlock* temp = reinterpret_cast< FreeBlock* >( block_start );
        temp->size = block_size;
        temp->next = previousFreeBlock->next;
        previousFreeBlock->next = temp;
        previousFreeBlock = temp;
    }

    // TODO: Crashes here
    if ( freeBlock != nullptr && reinterpret_cast< uptr >( freeBlock ) == block_end ) {
        previousFreeBlock += freeBlock->size;
        previousFreeBlock->next = freeBlock->next;
    }

    m_numAllocations--;
    m_usedMemory -= block_size;
}

}