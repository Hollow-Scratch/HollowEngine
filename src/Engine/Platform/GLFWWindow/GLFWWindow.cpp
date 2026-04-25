#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include "GLFWWindow.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

namespace Raycaster {

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

    if (!glfwInit()) {
        std::cerr << "[GLFW] Init failed\n";
        std::abort();
    }

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

    if (!gladLoadGL(glfwGetProcAddress)) {
        std::cerr << "[GLAD] Failed to initialize\n";
        std::abort();
    }

    std::cout << "[OpenGL] Version: " << glGetString(GL_VERSION) << "\n";

    glfwSwapInterval(m_Data.VSync ? 1 : 0);

    std::cout << "[GLFW] Window created\n";
}

void GLFWWindow::OnUpdate() {
    glfwPollEvents();
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