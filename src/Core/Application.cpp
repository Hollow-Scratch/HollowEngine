#include "Core/Application.hpp"
#include <iostream>

namespace Axiom
{
    Application::Application()
        : m_Window(Window::Create())
    {
    }

    void Application::run()
    {
        while (m_isRunning && !m_Window->ShouldClose())
        {
            std::cout << "Running Axiom Engine..." << std::endl;
            m_Window->OnUpdate();
        }
    }
}