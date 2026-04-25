#include "Window.hpp"
#include "Platform/GLFWWindow/GLFWWindow.hpp"

namespace Hollow {

std::unique_ptr<Window> Window::Create(const WindowProps& props) {
    return std::make_unique<GLFWWindow>(props);
}

}
