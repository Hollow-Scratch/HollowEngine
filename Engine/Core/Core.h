#pragma once

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
#endif