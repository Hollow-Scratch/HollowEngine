#pragma once

#include "Core/Window.h"

#include <string>

struct GLFWwindow;

namespace Axiom
{
    class GLFWWindow : public Window
    {
    public:
        explicit GLFWWindow(const WindowProps& props);
        ~GLFWWindow() override;

        void OnUpdate() override;

        std::uint32_t GetWidth() const override;
        std::uint32_t GetHeight() const override;
        bool ShouldClose() const override;

        void SetEventCallback(const EventCallbackFn& callback) override;

        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void* GetNativeWindow() const override;

    private:
        void Init(const WindowProps& props);
        void Shutdown();

        static void WindowSizeCallback(GLFWwindow* window, int width, int height);
        static void WindowCloseCallback(GLFWwindow* window);

    private:
        struct WindowData
        {
            std::string Title;
            std::uint32_t Width = 0;
            std::uint32_t Height = 0;
            bool VSync = false;
            bool ShouldClose = false;
            EventCallbackFn EventCallback;
        };

    private:
        GLFWwindow* m_Window = nullptr;
        WindowData m_Data{};
    };
}
