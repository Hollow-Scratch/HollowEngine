#include <glad/glad.h>
#include "GLFWWindow.hpp"
#include <GLFW/glfw3.h>
#include "Core/Input.hpp"
#include "Core/Keycodes.hpp"
#include <iostream>

namespace Hollow {

GLFWWindow::GLFWWindow(const WindowProps& props) {
    Init(props);
}

GLFWWindow::~GLFWWindow() {
    Shutdown();
}

void GLFWWindow::Init(const WindowProps& props) {
    m_Data.Title  = props.Title;
    m_Data.Width  = props.Width;
    m_Data.Height = props.Height;
    m_Data.VSync  = props.VSync;

    if (glfwInit() == GLFW_FALSE) {
        std::cerr << "[GLFW] Init failed\n";
        std::abort();
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    m_Window = glfwCreateWindow(
        m_Data.Width,
        m_Data.Height,
        m_Data.Title.c_str(),
        nullptr,
        nullptr
    );

    if (!m_Window) {
        std::cerr << "[GLFW] Window creation failed\n";
        glfwTerminate();
        std::abort();
    }

    glfwMakeContextCurrent(m_Window);

    if (glfwGetCurrentContext() != m_Window) {
        std::cerr << "[GLFW] Failed to make OpenGL context current\n";
        std::abort();
    }

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "[GLAD] Failed to initialize\n";
        std::abort();
    }

    if (glad_glGetString == nullptr) {
        std::cerr << "[GLAD] glGetString is NULL\n";
        std::abort();
    }

    std::cout << "[OpenGL] Version: "  << glGetString(GL_VERSION)  << "\n";
    std::cout << "[OpenGL] Vendor: "   << glGetString(GL_VENDOR)   << "\n";
    std::cout << "[OpenGL] Renderer: " << glGetString(GL_RENDERER) << "\n";

    glfwSwapInterval(m_Data.VSync ? 1 : 0);

    glfwSetWindowUserPointer(m_Window, &m_Data);

    glfwSetFramebufferSizeCallback(m_Window,
    [](GLFWwindow* window, int width, int height)
    {
        auto& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
        data.Width  = width;
        data.Height = height;
        glViewport(0, 0, width, height);
    });

    // For now
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(m_Window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glViewport(0, 0, m_Data.Width, m_Data.Height);
}

void GLFWWindow::OnUpdate() {
    glfwPollEvents();

    // For now, toggle cursor on Escape
    static bool pressed = false;

    if (Input::IsKeyPressed(Key_Escape)) {
        if (!pressed) {
            pressed = true;

            int mode = glfwGetInputMode(m_Window, GLFW_CURSOR);

            if (mode == GLFW_CURSOR_DISABLED)
                glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            else
                glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
    } else {
        pressed = false;
    }

    glfwSwapBuffers(m_Window);
}

void GLFWWindow::Shutdown() {
    if (m_Window) {
        glfwDestroyWindow(m_Window);
        m_Window = nullptr;
    }

    glfwTerminate();
}

uint32_t GLFWWindow::GetWidth() const { return m_Data.Width; }
uint32_t GLFWWindow::GetHeight() const { return m_Data.Height; }

void GLFWWindow::SetVSync(bool enabled) {
    m_Data.VSync = enabled;
    glfwSwapInterval(enabled ? 1 : 0);
}

bool GLFWWindow::IsVSync() const { return m_Data.VSync; }

void GLFWWindow::SetEventCallback(const EventCallbackFn& callback) {
    m_Data.EventCallback = callback;
}

void* GLFWWindow::GetNativeWindow() const {
    return m_Window;
}

bool GLFWWindow::ShouldClose() const {
    return glfwWindowShouldClose(m_Window);
}

}