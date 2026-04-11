#pragma once


// Platform Detection

#if defined(_WIN32) || defined(_WIN64)
    #define AXIOM_PLATFORM_WINDOWS 1
#elif defined(__APPLE__) && defined(__MACH__)
    #include <TargetConditionals.h>

    #if TARGET_OS_IPHONE
        #define AXIOM_PLATFORM_IOS 1
    #elif TARGET_OS_MAC
        #define AXIOM_PLATFORM_MAC 1
    #else
        #error "Unknown Apple platform"
    #endif

#elif defined(__linux__)
    #define AXIOM_PLATFORM_LINUX 1

#else
    #error "Unsupported platform"
#endif


// Architecture Detection

#if defined(_WIN64) || defined(__x86_64__) || defined(__aarch64__)
    #define AXIOM_ARCH_64BIT 1
#else
    #define AXIOM_ARCH_32BIT 1
#endif

// Compiler Detection

#if defined(_MSC_VER)
    #define AXIOM_COMPILER_MSVC 1
#elif defined(__clang__)
    #define AXIOM_COMPILER_CLANG 1
#elif defined(__GNUC__)
    #define AXIOM_COMPILER_GCC 1
#else
    #error "Unknown compiler"
#endif

// Debug / Release

#if defined(NDEBUG)
    #define AXIOM_CONFIG_RELEASE 1
#else
    #define AXIOM_CONFIG_DEBUG 1
#endif

// check

#if defined(AXIOM_PLATFORM_WINDOWS) + \
    defined(AXIOM_PLATFORM_MAC) +     \
    defined(AXIOM_PLATFORM_LINUX) +   \
    defined(AXIOM_PLATFORM_IOS) > 1
    #error "Multiple platforms detected!"
#endif