#pragma once

<<<<<<< HEAD:src/Core/Core.h
#include "PlatformDetection.h"

// ==========================
// DLL Export / Import
// ==========================

#if AXIOM_PLATFORM_WINDOWS

    #if defined(AXIOM_BUILD_DLL)
        #define AXIOM_API __declspec(dllexport)
    #else
        #define AXIOM_API __declspec(dllimport)
    #endif

#else
    // Linux / macOS (GCC/Clang)
    #define AXIOM_API __attribute__((visibility("default")))
=======
// Export macro
#if defined(AXIOM_ENGINE_EXPORTS)
    #if defined(_WIN32)
        #define AXIOM_API __declspec(dllexport)
    #else
        #define AXIOM_API __attribute__((visibility("default")))
    #endif
#else
    #if defined(_WIN32)
        #define AXIOM_API __declspec(dllimport)
    #else
        #define AXIOM_API
    #endif
>>>>>>> f26854c164ea88d1369bf8ff54d89440afeb21e5:Engine/Core/Core.h
#endif