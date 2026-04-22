#include "X11Window.hpp"
#include <iostream>

namespace Axiom
{
    X11Window::X11Window(const WindowProps& props)
    {
        Init(props);
    }

    X11Window::~X11Window()
    {
        Shutdown();
    }

    void X11Window::Init(const WindowProps& props)
    {
        m_Props = props;

        m_Display = XOpenDisplay(nullptr);
        if (!m_Display)
        {
            std::cerr << "Failed to open X11 display\n";
            return;
        }

        int screen = DefaultScreen(m_Display);

        m_Window = XCreateSimpleWindow(
            m_Display,
            RootWindow(m_Display, screen),
            0, 0,
            m_Props.Width,
            m_Props.Height,
            1,
            BlackPixel(m_Display, screen),
            WhitePixel(m_Display, screen)
        );

        XStoreName(m_Display, m_Window, m_Props.Title.c_str());

        XSelectInput(m_Display, m_Window,
            ExposureMask |
            KeyPressMask |
            KeyReleaseMask |
            ButtonPressMask |
            StructureNotifyMask
        );

        XMapWindow(m_Display, m_Window);

        m_WMDelete = XInternAtom(m_Display, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(m_Display, m_Window, &m_WMDelete, 1);

        m_GC = XDefaultGC(m_Display, screen);
    }

    void X11Window::OnUpdate()
    {
        while (XPending(m_Display))
        {
            XEvent event;
            XNextEvent(m_Display, &event);

            if (m_Callback)
                m_Callback();

            if (event.type == ClientMessage)
            {
                if ((Atom)event.xclient.data.l[0] == m_WMDelete)
                {
                    m_Running = false;
                }
            }
        }

        // Clear screen (engine placeholder render) for now
        XSetForeground(m_Display, m_GC, 0x202020);

        XFillRectangle(
            m_Display,
            m_Window,
            m_GC,
            0, 0,
            m_Props.Width,
            m_Props.Height
        );
    }

    uint32_t X11Window::GetWidth() const
    {
        return m_Props.Width;
    }

    uint32_t X11Window::GetHeight() const
    {
        return m_Props.Height;
    }

    bool X11Window::ShouldClose() const
    {
        return !m_Running;
    }

    void X11Window::SetEventCallback(const EventCallbackFn& callback)
    {
        m_Callback = callback;
    }

    void X11Window::SetVSync(bool enabled)
    {
        m_VSync = enabled;
    }

    bool X11Window::IsVSync() const
    {
        return m_VSync;
    }

    void* X11Window::GetNativeWindow() const
    {
        return (void*)&m_Window;
    }

    void X11Window::Shutdown()
    {
        if (m_Display)
        {
            XDestroyWindow(m_Display, m_Window);
            XCloseDisplay(m_Display);
        }
    }
}