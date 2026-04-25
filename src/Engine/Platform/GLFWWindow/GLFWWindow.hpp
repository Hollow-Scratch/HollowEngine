#pragma once

#include "Core/Window.hpp"

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

namespace Raycaster {

class GLFWWindow : public Window {
public:
    GLFWWindow(const WindowProps& props);
    ~GLFWWindow() override;

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

    struct Data {
        std::string Title;
        uint32_t Width = 0;
        uint32_t Height = 0;
        bool VSync = true;

        EventCallbackFn EventCallback;
    } m_Data;
};

}