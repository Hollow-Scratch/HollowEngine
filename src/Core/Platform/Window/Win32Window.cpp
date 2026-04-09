#include "Platform/Window/Win32Window.h"

#include <stdexcept>
#include <utility>

namespace
{
    constexpr const wchar_t* kWindowClassName = L"AxiomWindowClass";
}

namespace Axiom
{
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
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;
        m_ShouldClose = false;

        m_Instance = GetModuleHandleW(nullptr);

        WNDCLASSEXW windowClass{};
        windowClass.cbSize = sizeof(WNDCLASSEXW);
        windowClass.style = CS_HREDRAW | CS_VREDRAW;
        windowClass.lpfnWndProc = Win32Window::WndProc;
        windowClass.hInstance = m_Instance;
        windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
        windowClass.hbrBackground = static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
        windowClass.lpszClassName = kWindowClassName;

        static bool s_ClassRegistered = false;
        if (!s_ClassRegistered)
        {
            if (!RegisterClassExW(&windowClass))
            {
                throw std::runtime_error("Failed to register Win32 window class.");
            }

            s_ClassRegistered = true;
        }

        RECT windowRect{ 0, 0, static_cast<LONG>(m_Data.Width), static_cast<LONG>(m_Data.Height) };
        AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, FALSE);

        std::wstring wideTitle(m_Data.Title.begin(), m_Data.Title.end());

        m_WindowHandle = CreateWindowExW(
            0,
            kWindowClassName,
            wideTitle.c_str(),
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            windowRect.right - windowRect.left,
            windowRect.bottom - windowRect.top,
            nullptr,
            nullptr,
            m_Instance,
            this
        );

        if (m_WindowHandle == nullptr)
        {
            throw std::runtime_error("Failed to create Win32 window.");
        }

        ShowWindow(m_WindowHandle, SW_SHOW);
        UpdateWindow(m_WindowHandle);
    }

    void Win32Window::Shutdown()
    {
        if (m_WindowHandle != nullptr)
        {
            DestroyWindow(m_WindowHandle);
            m_WindowHandle = nullptr;
        }

        m_ShouldClose = true;
    }

    void Win32Window::OnUpdate()
    {
        MSG message{};
        while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&message);
            DispatchMessageW(&message);
        }
    }

    std::uint32_t Win32Window::GetWidth() const
    {
        return m_Data.Width;
    }

    std::uint32_t Win32Window::GetHeight() const
    {
        return m_Data.Height;
    }

    bool Win32Window::ShouldClose() const
    {
        return m_ShouldClose;
    }

    void Win32Window::SetEventCallback(const EventCallbackFn& callback)
    {
        m_Data.EventCallback = callback;
    }

    void Win32Window::SetVSync(bool enabled)
    {
        m_Data.VSync = enabled;
    }

    bool Win32Window::IsVSync() const
    {
        return m_Data.VSync;
    }

    void* Win32Window::GetNativeWindow() const
    {
        return m_WindowHandle;
    }

    LRESULT CALLBACK Win32Window::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        Win32Window* window = nullptr;

        if (message == WM_NCCREATE)
        {
            const auto* createStruct = reinterpret_cast<CREATESTRUCTW*>(lParam);
            window = static_cast<Win32Window*>(createStruct->lpCreateParams);
            SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
            window->m_WindowHandle = hwnd;
        }
        else
        {
            window = reinterpret_cast<Win32Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
        }

        switch (message)
        {
        case WM_SIZE:
        {
            if (window != nullptr)
            {
                window->m_Data.Width = LOWORD(lParam);
                window->m_Data.Height = HIWORD(lParam);
            }

            InvalidateRect(hwnd, nullptr, TRUE);
            return 0;
        }

        case WM_ERASEBKGND:
        {
            RECT rect{};
            GetClientRect(hwnd, &rect);
            FillRect(reinterpret_cast<HDC>(wParam), &rect, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
            return 1;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT paintStruct{};
            HDC hdc = BeginPaint(hwnd, &paintStruct);
            FillRect(hdc, &paintStruct.rcPaint, static_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
            EndPaint(hwnd, &paintStruct);
            return 0;
        }

        case WM_CLOSE:
        {
            if (window != nullptr)
            {
                window->m_ShouldClose = true;
            }

            DestroyWindow(hwnd);
            return 0;
        }

        case WM_DESTROY:
        {
            if (window != nullptr)
            {
                window->m_ShouldClose = true;
                window->m_WindowHandle = nullptr;
            }

            PostQuitMessage(0);
            return 0;
        }

        default:
            return DefWindowProcW(hwnd, message, wParam, lParam);
        }
    }
}