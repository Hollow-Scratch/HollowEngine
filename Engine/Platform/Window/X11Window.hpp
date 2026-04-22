#pragma once

#include "Core/Window.hpp"
#include <X11/Xlib.h>

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

    private:
        Display* m_Display = nullptr;
        ::Window m_Window;
        Atom m_WMDelete;
        GC m_GC;

        WindowProps m_Props;
        bool m_Running = true;

        EventCallbackFn m_Callback;
        bool m_VSync = false;
    };
}