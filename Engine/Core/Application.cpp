#include "Core/Application.hpp"
#include "Core/Time.hpp"


namespace Axiom
{
    Application::Application()
        : m_Window(Window::Create())
    {
    }

    void Application::run()
    {
        OnInit();

<<<<<<< HEAD:src/Core/Application.cpp
        while (m_isRunning && !m_Window->ShouldClose())
        {
            // Engine tick
=======
        Time::Reset();

        while (m_isRunning && !m_Window->ShouldClose())
        {
            Time::Update();
            OnUpdate(Time::GetDeltaTime());
>>>>>>> f26854c164ea88d1369bf8ff54d89440afeb21e5:Engine/Core/Application.cpp
            m_Window->OnUpdate();

            // Client update
            OnUpdate();
        }

        OnShutdown();
    }
}