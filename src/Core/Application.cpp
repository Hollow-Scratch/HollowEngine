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
        OnInit();

        while (m_isRunning && !m_Window->ShouldClose())
        {
            // Engine tick
            m_Window->OnUpdate();

            // Client update
            OnUpdate();
        }

        OnShutdown();
    }
}