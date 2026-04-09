#pragma once

#include "Core/Window.h"

#include <Windows.h>
#include <string>

namespace Axiom
{
    class Win32Window : public Window
    {
    public:
        explicit Win32Window(const WindowProps& props);
        ~Win32Window() override;

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

        static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

    private:
        struct WindowData
        {
            std::string Title;
            std::uint32_t Width = 0;
            std::uint32_t Height = 0;
            bool VSync = false;
            EventCallbackFn EventCallback;
        };

    private:
        HINSTANCE m_Instance = nullptr;
        HWND m_WindowHandle = nullptr;
        WindowData m_Data{};
        bool m_ShouldClose = false;
    };
}