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

        Time::Reset();

        while (m_isRunning && !m_Window->ShouldClose())
        {
            Time::Update();
            OnUpdate(Time::GetDeltaTime());
            m_Window->OnUpdate();
        }

        OnShutdown();
    }
}