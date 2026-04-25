#include "Application.hpp"
#include "Window.hpp"
#include "Time.hpp"
#include "Input.hpp"
#include <iostream>

namespace Hollow
{

Application* Application::s_Instance = nullptr;

Application::Application()
{
    s_Instance = this;
}

Application::~Application() noexcept = default;

void Application::Run()
{
    m_Window = Window::Create();

    OnInit();

    Hollow::Time time;

    float timer = 0.0f;
    int frames = 0;

    while (m_Window && !m_Window->ShouldClose())
    {
        time.beginFrame();

        float DeltaTime = time.getScaledDeltaTime();

        Input::Update();

        OnUpdate(DeltaTime);

        m_Window->OnUpdate();

        timer += DeltaTime;
        frames++;

        if (timer >= 1.0f)
        {
            float fps = static_cast<float>(frames) / timer;
            std::cout << "FPS: " << fps << std::endl;

            timer = 0.0f;
            frames = 0;
        }

        time.endFrame();
    }

    if (m_Window)
    {
        m_Window->Shutdown();
        m_Window.reset();
    }

    OnShutdown();
}

}
