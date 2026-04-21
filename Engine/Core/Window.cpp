#include "Window.hpp"
#include "Platform/Window/GLFWWindow.hpp"

namespace Axiom
{
    std::unique_ptr<Window> Window::Create(const WindowProps& props)
    {
        return std::make_unique<GLFWWindow>(props);
    }
}
