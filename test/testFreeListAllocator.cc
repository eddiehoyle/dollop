#include <gtest/gtest.h>
#include <utility>

#include <DLib/FreeListAllocator.hh>
#include <DLib/Math.hh>

template< typename T >
inline T* create( dollop::Allocator& allocator ) {
    return reinterpret_cast< T* >( allocator.allocate( sizeof( T ), alignof( T ) ) );
}

struct Image {
    dollop::u32 width;
    dollop::u32 height;
};

class TestFreeListAllocator : public ::testing::Test {
protected:
    virtual void SetUp() {
        m_size = 1024 * 1024;
        m_memory = malloc( m_size );

        std::size_t size = m_size - sizeof( dollop::FreeListAllocator );
        void* start = dollop::pointer_math::add( m_memory, sizeof( dollop::FreeListAllocator ) );
        m_allocator = new( m_memory )dollop::FreeListAllocator( size, start );
    }

    virtual void TearDown() {
        free( m_memory );
        m_allocator = nullptr;
        m_memory = nullptr;
        m_size = 0;
    }

    std::size_t m_size;
    void* m_memory;
    dollop::FreeListAllocator* m_allocator;
};

TEST_F( TestFreeListAllocator, ctor ) {

    using namespace dollop;

    EXPECT_TRUE( m_allocator != nullptr );
}

TEST_F( TestFreeListAllocator, empty_size ) {

    using namespace dollop;

    EXPECT_TRUE( m_allocator != nullptr );
    EXPECT_EQ( m_allocator->getCount(), 0 );
}


TEST_F( TestFreeListAllocator, allocate_one ) {

    using namespace dollop;

    Image* image = create< Image >( *m_allocator );
    EXPECT_TRUE( image != nullptr );
    EXPECT_EQ( m_allocator->getCount(), 1 );
}

TEST_F( TestFreeListAllocator, allocate_many ) {

    using namespace dollop;

    Image* imageA = create< Image >( *m_allocator );
    EXPECT_TRUE( imageA != nullptr );
    EXPECT_EQ( m_allocator->getCount(), 1 );

    Image* imageB = create< Image >( *m_allocator );
    EXPECT_TRUE( imageB != nullptr );
    EXPECT_TRUE( imageB != imageA );
    EXPECT_EQ( m_allocator->getCount(), 2 );

    Image* imageC = create< Image >( *m_allocator );
    EXPECT_TRUE( imageC != nullptr );
    EXPECT_TRUE( imageC != imageB );
    EXPECT_TRUE( imageC != imageA );
    EXPECT_EQ( m_allocator->getCount(), 3 );
}

TEST_F( TestFreeListAllocator, deallocate ) {

    using namespace dollop;

    std::size_t used = m_allocator->getUsedMemory();

    Image* imageA = create< Image >( *m_allocator );
    EXPECT_TRUE( imageA != nullptr );
    EXPECT_NE( m_allocator->getUsedMemory(), used );

    m_allocator->deallocate( imageA );
    EXPECT_EQ( m_allocator->getCount(), 0 );
    EXPECT_EQ( m_allocator->getUsedMemory(), used );



}