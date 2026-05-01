#include "Application.hpp"
#include "Window.hpp"
#include "Input.hpp"
#include "Renderer/Renderer.hpp"
#include "Renderer/Framebuffer.hpp"
#include <iostream>
#include <algorithm>

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
    m_Window->SetVSync(false);

    Renderer::Init();
    OnInit();

    while (m_Window && !m_Window->ShouldClose())
    {
        m_Time.beginFrame();

        float DeltaTime = m_Time.getScaledDeltaTime();

        static float minDt = 1000.0f;
        static float maxDt = 0.0f;
        static float sumDt = 0.0f;
        static int frameCount = 0;

        minDt = std::min(minDt, DeltaTime);
        maxDt = std::max(maxDt, DeltaTime);
        sumDt += DeltaTime;
        frameCount++;

        if (frameCount >= 60)
        {
            float avgDt = sumDt / frameCount;
            float fps = 1.0f / avgDt;

            std::cout
                << "min: " << minDt * 1000.0f << " ms, "
                << "avg: " << avgDt * 1000.0f << " ms, "
                << "max: " << maxDt * 1000.0f << " ms, "
                << "FPS(avg): " << fps
                << std::endl;

            minDt = 1000.0f;
            maxDt = 0.0f;
            sumDt = 0.0f;
            frameCount = 0;
        }

        Input::Update();

        Renderer::Clear(0.1f, 0.1f, 0.2f);
        OnUpdate(DeltaTime);

        m_Window->OnUpdate();

        m_Time.endFrame();
    }

    if (m_Window)
    {
        Framebuffer::MarkContextDestroyed();
        m_Window->Shutdown();
        m_Window.reset();
    }

    OnShutdown();
}

}