#include <gtest/gtest.h>
#include <utility>

#include <DLib/StackAllocator.hh>
#include <DLib/Math.hh>

template< typename T >
inline T* create( dollop::Allocator& allocator ) {
    return reinterpret_cast< T* >( allocator.allocate( sizeof( T ), alignof( T ) ) );
}

struct Image {
    dollop::u32 width;
    dollop::u32 height;
};

class TestStackAllocator : public ::testing::Test {
protected:
    virtual void SetUp() {
        m_size = 1024 * 1024;
        m_memory = malloc( m_size );

        std::size_t size = m_size - sizeof( dollop::StackAllocator );
        void* start = dollop::pointer_math::add( m_memory, sizeof( dollop::StackAllocator ) );
        m_allocator = new( m_memory )dollop::StackAllocator( size, start );
    }

    virtual void TearDown() {
        free( m_memory );
        m_allocator = nullptr;
        m_memory = nullptr;
        m_size = 0;
    }

    std::size_t m_size;
    void* m_memory;
    dollop::StackAllocator* m_allocator;
};

TEST_F( TestStackAllocator, ctor ) {

    using namespace dollop;

    EXPECT_TRUE( m_allocator != nullptr );
}

TEST_F( TestStackAllocator, empty_size ) {

    using namespace dollop;

    EXPECT_TRUE( m_allocator != nullptr );
    EXPECT_EQ( m_allocator->getCount(), 0 );
}

TEST_F( TestStackAllocator, empty_top ) {

    using namespace dollop;

    EXPECT_TRUE( m_allocator != nullptr );
    EXPECT_TRUE( m_allocator->top() == nullptr );
}

TEST_F( TestStackAllocator, allocate_one ) {

    using namespace dollop;

    Image* image = create< Image >( *m_allocator );
    EXPECT_TRUE( image != nullptr );
    EXPECT_EQ( image, m_allocator->top() );
    EXPECT_EQ( m_allocator->getCount(), 1 );
}

TEST_F( TestStackAllocator, allocate_many ) {

    using namespace dollop;

    Image* imageA = create< Image >( *m_allocator );
    EXPECT_TRUE( imageA != nullptr );
    EXPECT_EQ( imageA, m_allocator->top() );
    EXPECT_EQ( m_allocator->getCount(), 1 );

    Image* imageB = create< Image >( *m_allocator );
    EXPECT_TRUE( imageB != nullptr );
    EXPECT_TRUE( imageB != imageA );
    EXPECT_EQ( imageB, m_allocator->top() );
    EXPECT_EQ( m_allocator->getCount(), 2 );

    Image* imageC = create< Image >( *m_allocator );
    EXPECT_TRUE( imageC != nullptr );
    EXPECT_TRUE( imageC != imageB );
    EXPECT_TRUE( imageC != imageA );
    EXPECT_EQ( imageC, m_allocator->top() );
    EXPECT_EQ( m_allocator->getCount(), 3 );
}

TEST_F( TestStackAllocator, deallocate ) {

    using namespace dollop;
    Image* imageA = create< Image >( *m_allocator );
    EXPECT_TRUE( imageA != nullptr );
    m_allocator->deallocate( imageA );

}