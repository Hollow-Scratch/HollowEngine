#include "Application.hpp"
#include "Window.hpp"
#include "Time.hpp"
#include "Input.hpp"
#include <iostream>

namespace Raycaster
{

Application* Application::s_Instance = nullptr;

Application::Application()
{
    s_Instance = this;
}

Application::~Application() noexcept = default;

void Application::Run()
{
    OnInit();

    m_Window = Window::Create();

    Raycaster::Time time;

    float timer = 0.0f;
    int frames = 0;

    while (m_Window && !m_Window->ShouldClose())
    {
        time.beginFrame();

        float dt = time.getScaledDeltaTime();

        Input::Update();

        OnUpdate(dt);

        m_Window->OnUpdate();

        timer += dt;
        frames++;

        if (timer >= 1.0f)
        {
            float fps = frames / timer;
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