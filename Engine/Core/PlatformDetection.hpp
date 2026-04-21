#pragma once

// Platform
#if defined(_WIN32) || defined(_WIN64)
    #define AXIOM_PLATFORM_WINDOWS 1
#elif defined(__linux__)
    #define AXIOM_PLATFORM_LINUX 1
#else
    #error "Unsupported platform"
#endif

// Architecture
#if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__)
    #define AXIOM_ARCH_64BIT 1
#else
    #define AXIOM_ARCH_32BIT 1
#endif

// Compiler
#if defined(_MSC_VER)
    #define AXIOM_COMPILER_MSVC 1
#elif defined(__clang__)
    #define AXIOM_COMPILER_CLANG 1
#elif defined(__GNUC__)
    #define AXIOM_COMPILER_GCC 1
#else
    #error "Unknown compiler"
#endif

// Build type
#if defined(NDEBUG)
    #define AXIOM_CONFIG_RELEASE 1
#else
    #define AXIOM_CONFIG_DEBUG 1
#endif



// Safety check
#if defined(AXIOM_PLATFORM_WINDOWS) && defined(AXIOM_PLATFORM_LINUX)
    #error "Multiple platforms detected!"
#endif