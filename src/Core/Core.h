#pragma once

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
#endif