#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Platform/Window/GLFWWindow.hpp"

#include <iostream>
#include <stdexcept>

namespace
{
    std::uint32_t s_GLFWWindowCount = 0;
}

namespace Axiom
{
    GLFWWindow::GLFWWindow(const WindowProps& props)
    {
        Init(props);
    }

    GLFWWindow::~GLFWWindow()
    {
        Shutdown();
    }

    void GLFWWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;
        m_Data.ShouldClose = false;

        const bool shouldTerminateOnFailure = s_GLFWWindowCount == 0;
        if (s_GLFWWindowCount == 0 && glfwInit() == GLFW_FALSE)
        {
            throw std::runtime_error("Failed to initialize GLFW.");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(
            static_cast<int>(m_Data.Width),
            static_cast<int>(m_Data.Height),
            m_Data.Title.c_str(),
            nullptr,
            nullptr
        );

        if (m_Window == nullptr)
        {
            if (shouldTerminateOnFailure)
            {
                glfwTerminate();
            }

            throw std::runtime_error("Failed to create GLFW window.");
        }

        glfwMakeContextCurrent(m_Window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cerr << "Failed to initialize GLAD." << std::endl;
            glfwDestroyWindow(m_Window);
            m_Window = nullptr;

            if (shouldTerminateOnFailure)
            {
                glfwTerminate();
            }

            throw std::runtime_error("Failed to initialize GLAD.");
        }

        ++s_GLFWWindowCount;

        glfwSetWindowUserPointer(m_Window, &m_Data);
        glfwSetWindowSizeCallback(m_Window, GLFWWindow::WindowSizeCallback);
        glfwSetWindowCloseCallback(m_Window, GLFWWindow::WindowCloseCallback);

        SetVSync(true);
    }

    void GLFWWindow::Shutdown()
    {
        if (m_Window != nullptr)
        {
            glfwDestroyWindow(m_Window);
            m_Window = nullptr;

            if (s_GLFWWindowCount > 0)
            {
                --s_GLFWWindowCount;
            }

            if (s_GLFWWindowCount == 0)
            {
                glfwTerminate();
            }
        }

        m_Data.ShouldClose = true;
    }

    void GLFWWindow::OnUpdate()
    {
        if (m_Window == nullptr)
        {
            return;
        }

        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    std::uint32_t GLFWWindow::GetWidth() const
    {
        return m_Data.Width;
    }

    std::uint32_t GLFWWindow::GetHeight() const
    {
        return m_Data.Height;
    }

    bool GLFWWindow::ShouldClose() const
    {
        return m_Window == nullptr || m_Data.ShouldClose || glfwWindowShouldClose(m_Window) == GLFW_TRUE;
    }

    void GLFWWindow::SetEventCallback(const EventCallbackFn& callback)
    {
        m_Data.EventCallback = callback;
    }

    void GLFWWindow::SetVSync(bool enabled)
    {
        m_Data.VSync = enabled;

        if (m_Window != nullptr)
        {
            glfwMakeContextCurrent(m_Window);
            glfwSwapInterval(enabled ? 1 : 0);
        }
    }

    bool GLFWWindow::IsVSync() const
    {
        return m_Data.VSync;
    }

    void* GLFWWindow::GetNativeWindow() const
    {
        return m_Window;
    }

    void GLFWWindow::WindowSizeCallback(GLFWwindow* window, int width, int height)
    {
        auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        if (data == nullptr)
        {
            return;
        }

        data->Width = static_cast<std::uint32_t>(width > 0 ? width : 0);
        data->Height = static_cast<std::uint32_t>(height > 0 ? height : 0);

        if (data->EventCallback)
        {
            data->EventCallback();
        }
    }

    void GLFWWindow::WindowCloseCallback(GLFWwindow* window)
    {
        auto* data = static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        if (data == nullptr)
        {
            return;
        }

        data->ShouldClose = true;

        if (data->EventCallback)
        {
            data->EventCallback();
        }
    }
}
