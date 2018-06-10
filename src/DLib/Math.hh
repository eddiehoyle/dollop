#ifndef DOLLOP_MATH_HH
#define DOLLOP_MATH_HH

#include "DLib/util/Platform.hh"

namespace dollop {

namespace pointer_math {

inline void* add( void* address, std::size_t distance ) {
    return static_cast< void* >( reinterpret_cast< u8* >( address ) + distance );
}

inline void* subtract( void* address, std::size_t distance ) {
    return static_cast< void* >( reinterpret_cast< u8* >( address ) - distance );
}

inline void* alignForward( void* address, u8 alignment ) {
    return reinterpret_cast< void* >(
            ( reinterpret_cast< uptr >( address ) + static_cast< uptr >( alignment - 1 ) )
            & static_cast< uptr >( ~( alignment - 1 ) ) );
}

inline u8 alignForwardAdjustment( const void* address, u8 alignment ) {
    u8 adjustment = static_cast< u8 >( alignment - ( reinterpret_cast< uptr >( address ) & static_cast< uptr >( alignment - 1 ) ) );
    if ( adjustment == alignment ) {
        return 0;
    }
    return adjustment;
}

template< typename T >
inline u8 alignForwardAdjustmentWithHeader( void* address, u8 alignment ) {
    if ( alignof( T ) > alignment ) {
        alignment = alignof( T );
    }
    u8 adjustment = sizeof( T ) + alignForwardAdjustment( add( address, sizeof( T ) ), alignment );
    return adjustment;
}

}

}

#endif //DOLLOP_MATH_HH
