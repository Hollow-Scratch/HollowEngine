#include "Window.h"

#if defined(_WIN32)
    #include "Platform/Window/Win32Window.h"
#elif defined(AXIOM_USE_X11)
    #include "Platform/Window/X11Window.h"
#elif defined(AXIOM_USE_WAYLAND)
    #include "Platform/Window/WaylandWindow.h"
#endif

namespace Axiom
{
    std::unique_ptr<Window> Window::Create(const WindowProps& props)
    {
    #if defined(_WIN32)
        return std::make_unique<Win32Window>(props);
    #elif defined(AXIOM_USE_X11)
        return std::make_unique<X11Window>(props);
    #elif defined(AXIOM_USE_WAYLAND)
        return std::make_unique<WaylandWindow>(props);
    #else
        return nullptr;
    #endif
    }
}