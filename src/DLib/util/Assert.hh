#ifndef DOLLOP_ASSERT_HH
#define DOLLOP_ASSERT_HH

#include "Log.hh"

#if DLP_ASSERT_ENABLED
#define STR(exp) #exp
#define DLP_ASSERT(exp)                                         \
do {                                                            \
    if ( !( exp ) ) {                                           \
        DLP_ERROR << "Assertion failed: (" << STR(exp) << ") "  \
                  << "[" << __FILE__ << ":" << __LINE__ << "]"; \
        abort();                                                \
    }                                                           \
} while(0);
#else
#define DLP_ASSERT(ignore)
#endif

#endif // DOLLOP_ASSERT_HH
