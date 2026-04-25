#pragma once

#include "Core/Window.hpp"
#include <GLFW/glfw3.h>

namespace Hollow {

class GLFWWindow : public Window {
public:
    GLFWWindow(const WindowProps& props);
    ~GLFWWindow();

    void OnUpdate() override;
    void Shutdown() override;

    uint32_t GetWidth() const override;
    uint32_t GetHeight() const override;

    void SetVSync(bool enabled) override;
    bool IsVSync() const override;

    void SetEventCallback(const EventCallbackFn& callback) override;

    void* GetNativeWindow() const override;

    bool ShouldClose() const override;

private:
    void Init(const WindowProps& props);

private:
    GLFWwindow* m_Window = nullptr;

    struct WindowData {
        std::string Title;
        uint32_t Width, Height;
        bool VSync;
        EventCallbackFn EventCallback;
    };

    WindowData m_Data;
};

}