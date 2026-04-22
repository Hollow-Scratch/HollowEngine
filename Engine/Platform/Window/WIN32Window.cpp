#include "WIN32Window.hpp"

namespace Axiom
{
    LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        if (msg == WM_CLOSE)
        {
            PostQuitMessage(0);
            return 0;
        }
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    Win32Window::Win32Window(const WindowProps& props)
    {
        Init(props);
    }

    Win32Window::~Win32Window()
    {
        Shutdown();
    }

    void Win32Window::Init(const WindowProps& props)
    {
        m_Props = props;

        HINSTANCE hInstance = GetModuleHandle(nullptr);

        WNDCLASS wc = {};
        wc.lpfnWndProc = WndProc;
        wc.hInstance = hInstance;
        wc.lpszClassName = L"AxiomWindowClass";

        RegisterClass(&wc);

        m_WindowHandle = CreateWindowEx(
            0,
            wc.lpszClassName,
            L"Axiom Engine",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, CW_USEDEFAULT,
            props.Width, props.Height,
            nullptr,
            nullptr,
            hInstance,
            nullptr
        );

        ShowWindow(m_WindowHandle, SW_SHOW);

        m_DeviceContext = GetDC(m_WindowHandle);
    }

    void Win32Window::OnUpdate()
    {
        MSG msg = {};
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                m_Running = false;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        // simple clear (like your X11 version)
        PatBlt(m_DeviceContext, 0, 0, m_Props.Width, m_Props.Height, BLACKNESS);
    }

    bool Win32Window::ShouldClose() const
    {
        return !m_Running;
    }

    uint32_t Win32Window::GetWidth() const
    {
        return m_Props.Width;
    }

    uint32_t Win32Window::GetHeight() const
    {
        return m_Props.Height;
    }

    void* Win32Window::GetNativeWindow() const
    {
        return (void*)m_WindowHandle;
    }

    void Win32Window::SetEventCallback(const EventCallbackFn&) {}

    void Win32Window::SetVSync(bool) {}

    bool Win32Window::IsVSync() const { return m_VSync; }

    void Win32Window::Shutdown()
    {
        if (m_WindowHandle)
            DestroyWindow(m_WindowHandle);
    }
}