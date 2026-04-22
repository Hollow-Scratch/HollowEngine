#pragma once

#include "Core/Window.hpp"

#include <X11/Xlib.h>
#include <functional>

namespace Axiom
{
    class X11Window : public Window
    {
    public:
        X11Window(const WindowProps& props);
        ~X11Window() override;

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

        void Render();

    private:
        Display* m_Display = nullptr;
        ::Window m_Window = 0;
        Atom m_WMDelete = 0;
        GC m_GC = nullptr;

        WindowProps m_Props;

        bool m_Running = true;
        bool m_VSync = false;

        bool m_Resized = false;

        EventCallbackFn m_Callback;
    };
}