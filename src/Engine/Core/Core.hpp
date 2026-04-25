#pragma once

#if defined(_WIN32)
    #if defined(RAYCASTER_BUILD_DLL)
        #define RAYCASTER_API __declspec(dllexport)
    #else
        #define RAYCASTER_API __declspec(dllimport)
    #endif
#else
    // Linux / macOS (GCC / Clang)
    #if defined(RAYCASTER_BUILD_DLL)
        #define RAYCASTER_API __attribute__((visibility("default")))
    #else
        #define RAYCASTER_API
    #endif
#endif