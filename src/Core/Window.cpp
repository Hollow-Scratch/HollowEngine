#include "Window.h"
#include "Platform/Window/GLFWWindow.h"

namespace Axiom
{
    std::unique_ptr<Window> Window::Create(const WindowProps& props)
    {
        return std::make_unique<GLFWWindow>(props);
    }
}
