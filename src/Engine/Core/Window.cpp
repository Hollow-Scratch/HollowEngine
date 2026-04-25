#include "Window.hpp"
#include "Platform/GLFWWindow/GLFWWindow.hpp"

namespace Raycaster {

std::unique_ptr<Window> Window::Create(const WindowProps& props) {
    return std::make_unique<GLFWWindow>(props);
}

}