#pragma once

#include "PlatformDetection.hpp"

// Debug Break

#if AXIOM_COMPILER_MSVC
    #define AXIOM_DEBUG_BREAK() __debugbreak()
#elif AXIOM_COMPILER_CLANG || AXIOM_COMPILER_GCC
    #include <signal.h>
    #define AXIOM_DEBUG_BREAK() raise(SIGTRAP)
#else
    #include <cstdlib>
    #define AXIOM_DEBUG_BREAK() std::abort()
#endif

// Assertions enabled/disabled

#if AXIOM_CONFIG_DEBUG
    #define AXIOM_ENABLE_ASSERTS 1
#else
    #define AXIOM_ENABLE_ASSERTS 0
#endif

// Assertion macro

#if AXIOM_ENABLE_ASSERTS

    #include <cstdio>
    #include <cstdlib>

    #define AXIOM_ASSERT(x, msg)                                      \
        do {                                                         \
            if (!(x)) [[unlikely]] {                                 \
                std::fprintf(stderr, "Assertion Failed: %s\n", msg); \
                std::fprintf(stderr, "File: %s\n", __FILE__);        \
                std::fprintf(stderr, "Line: %d\n", __LINE__);        \
                std::fprintf(stderr, "Function: %s\n", __func__);    \
                AXIOM_DEBUG_BREAK();                                 \
                std::abort();                                        \
            }                                                        \
        } while (0)

    #define AXIOM_ASSERT_NO_MSG(x) AXIOM_ASSERT(x, "Assertion failed")

#else

    #define AXIOM_ASSERT(x, msg) ((void)0)
    #define AXIOM_ASSERT_NO_MSG(x) ((void)0)

#endif