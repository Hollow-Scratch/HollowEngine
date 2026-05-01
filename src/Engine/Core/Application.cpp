#include "Application.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Renderer/Renderer.hpp"
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

    Renderer::Init();
    OnInit();

    float timer = 0.0f;
    int frames = 0;

    while (m_Window && !m_Window->ShouldClose())
    {
        m_Time.beginFrame();

        float DeltaTime = m_Time.getScaledDeltaTime();

        Input::Update();

        Renderer::Clear(0.1f, 0.1f, 0.2f);
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

        m_Time.endFrame();
    }

    if (m_Window)
    {
        m_Window->Shutdown();
        m_Window.reset();
    }

    OnShutdown();
}

}