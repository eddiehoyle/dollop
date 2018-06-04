#ifndef DOLLOP_PLATFORM_HH
#define DOLLOP_PLATFORM_HH

#include <stdlib.h>
#include <stdarg.h>
#include <stdint.h>
#include <limits>

#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>

#include <algorithm>
#include <functional>

#include <assert.h>
#include <memory>

#include <string>
#include <math.h>

#include <chrono>

namespace dollop {

// Check if using 64-Bit architecture
#if ( defined(_WIN64) ||     \
      defined(__x86_64__) || \
      defined(__ppc64__) ||  \
      defined(_M_AMD64) ||   \
      defined(_M_ARM64) ||   \
      defined(_M_X64) )
#define CONC_64BIT 1

// Check if using 32-Bit architecture
#elif ( defined(_WIN32) && !defined(_WIN64)) || defined(_M_IX86)
#define CONC_32BIT 1
#endif

// signed integer type
using i8			= int8_t;
using i16			= int16_t;
using i32			= int32_t;

#ifdef CONC_64BIT
using i64			= int64_t;
#else
using i64			= int32_t;
#endif

// unsigned integer type
using u8			= uint8_t;
using u16			= uint16_t;
using u32			= uint32_t;
#ifdef CONC_64BIT
using u64			= uint64_t;
#else
using u64			= uint32_t;
#endif

// floating point
using f32			= float_t;
using f64			= double_t;

// pointer
using iptr			= intptr_t;
using uptr			= uintptr_t;

} // namespace dollop

#endif // DOLLOP_PLATFORM_HH
