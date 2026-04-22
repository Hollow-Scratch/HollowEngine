#include "Window.hpp"

#if AXIOM_PLATFORM_WINDOWS
    #include "Platform/Window/Win32Window.hpp"
#elif AXIOM_PLATFORM_LINUX
    #include "Platform/Window/X11Window.hpp"
#else
    #error "Unsupported platform"
#endif

namespace Axiom
{
    std::unique_ptr<Window> Window::Create(const WindowProps& props)
    {
#if AXIOM_PLATFORM_WINDOWS
        return std::make_unique<Win32Window>(props);
#elif AXIOM_PLATFORM_LINUX
        return std::make_unique<X11Window>(props);
#else
        return nullptr; // safety fallback
#endif
    }
}
