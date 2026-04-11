#include "PlatformDetection.h"
#include "Window.h"

#if AXIOM_PLATFORM_WINDOWS
    #include "Platform/Window/Win32Window.h"
#elif AXIOM_PLATFORM_LINUX && defined(AXIOM_USE_X11)
    #include "Platform/Window/X11Window.h"
#elif AXIOM_PLATFORM_LINUX && defined(AXIOM_USE_WAYLAND)
    #include "Platform/Window/WaylandWindow.h"
#else
    #error Unsupported platform/window backend
#endif

namespace Axiom
{
    std::unique_ptr<Window> Window::Create(const WindowProps& props)
    {
    #if AXIOM_PLATFORM_WINDOWS
        return std::make_unique<Win32Window>(props);
    #elif AXIOM_PLATFORM_LINUX && defined(AXIOM_USE_X11)
        return std::make_unique<X11Window>(props);
    #elif AXIOM_PLATFORM_LINUX && defined(AXIOM_USE_WAYLAND)
        return std::make_unique<WaylandWindow>(props);
    #else
        return nullptr;
    #endif
    }
}