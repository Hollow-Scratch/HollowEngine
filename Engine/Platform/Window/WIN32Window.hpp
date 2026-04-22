#pragma once

#include "Core/Window.hpp"
#include <windows.h>

namespace Axiom
{
    class Win32Window : public Window
    {
    public:
        Win32Window(const WindowProps& props);
        ~Win32Window() override;

        void OnUpdate() override;

        uint32_t GetWidth() const override;
        uint32_t GetHeight() const override;
        bool ShouldClose() const override;

        void SetEventCallback(const EventCallbackFn& callback) override;

        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void* GetNativeWindow() const override;

    private:
        void Init(const WindowProps& props);
        void Shutdown();

    private:
        HWND m_WindowHandle = nullptr;
        HDC m_DeviceContext = nullptr;
        bool m_Running = true;

        WindowProps m_Props;
        EventCallbackFn m_Callback;
        bool m_VSync = false;
    };#pragma once

#include "Core/Window.hpp"
#include <windows.h>

namespace Axiom
{
    class Win32Window : public Window
    {
    public:
        Win32Window(const WindowProps& props);
        ~Win32Window() override;

        void OnUpdate() override;

        uint32_t GetWidth() const override;
        uint32_t GetHeight() const override;
        bool ShouldClose() const override;

        void SetEventCallback(const EventCallbackFn& callback) override;

        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void* GetNativeWindow() const override;

    private:
        void Init(const WindowProps& props);
        void Shutdown();

    private:
        HWND m_WindowHandle = nullptr;
        HDC m_DeviceContext = nullptr;
        bool m_Running = true;

        WindowProps m_Props;
        EventCallbackFn m_Callback;
        bool m_VSync = false;
    };
}
}