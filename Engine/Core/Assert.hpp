#pragma once

#include "PlatformDetection.hpp"

// Debug Break

#if AXIOM_COMPILER_MSVC
    #define AXIOM_DEBUG_BREAK() __debugbreak()
#elif AXIOM_COMPILER_CLANG || AXIOM_COMPILER_GCC
    #include <signal.h>
    #define AXIOM_DEBUG_BREAK() raise(SIGTRAP)
#else
    #define AXIOM_DEBUG_BREAK()
#endif

// Enable asserts only in debug

#if AXIOM_CONFIG_DEBUG
    #define AXIOM_ENABLE_ASSERTS 1
#else
    #define AXIOM_ENABLE_ASSERTS 0
#endif

// Assert Macro

#if AXIOM_ENABLE_ASSERTS

    #include <iostream>
    #include <cstdlib>

    #define AXIOM_ASSERT(x, msg)                                     \
        do {                                                         \
            if (!(x)) {                                              \
                std::cerr << "Assertion Failed: " << msg << "\n";    \
                std::cerr << "File: " << __FILE__ << "\n";           \
                std::cerr << "Line: " << __LINE__ << "\n";           \
                std::cerr << "Function: " << __FUNCTION__ << "\n";   \
                AXIOM_DEBUG_BREAK();                                 \
                std::abort();                                        \
            }                                                        \
        } while (0)

    #define AXIOM_ASSERT_NO_MSG(x) AXIOM_ASSERT(x, "Assertion failed")

#else

    #define AXIOM_ASSERT(x, msg)
    #define AXIOM_ASSERT_NO_MSG(x)

#endif