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
        m_Running = true;

        m_Display = XOpenDisplay(nullptr);
        if (!m_Display)
        {
            std::cerr << "Failed to open X11 display\n";
            m_Running = false;
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

        if (!m_Window)
        {
            std::cerr << "Failed to create X11 window\n";
            m_Running = false;
            return;
        }

        XStoreName(m_Display, m_Window, m_Props.Title.c_str());

        XSelectInput(m_Display, m_Window,
            ExposureMask |
            KeyPressMask |
            KeyReleaseMask |
            ButtonPressMask |
            StructureNotifyMask
        );

        // Window close handling
        m_WMDelete = XInternAtom(m_Display, "WM_DELETE_WINDOW", False);
        XSetWMProtocols(m_Display, m_Window, &m_WMDelete, 1);

        XMapWindow(m_Display, m_Window);

        m_GC = XDefaultGC(m_Display, screen);
    }

    void X11Window::OnUpdate()
    {
        m_Resized = false;

        // --- Process ALL events first ---
        while (XPending(m_Display))
        {
            XEvent event;
            XNextEvent(m_Display, &event);

            switch (event.type)
            {
                case ClientMessage:
                {
                    if ((Atom)event.xclient.data.l[0] == m_WMDelete)
                        m_Running = false;
                    break;
                }

                case ConfigureNotify:
                {
                    uint32_t newWidth  = event.xconfigure.width;
                    uint32_t newHeight = event.xconfigure.height;

                    if (newWidth != m_Props.Width || newHeight != m_Props.Height)
                    {
                        m_Props.Width  = newWidth;
                        m_Props.Height = newHeight;
                        m_Resized = true;
                    }
                    break;
                }

                case Expose:
                {
                    // X11 requests redraw (handled by Render below)
                    break;
                }

                case KeyPress:
                case KeyRelease:
                case ButtonPress:
                {
                    // TODO: input system later
                    break;
                }
            }

            // Simple callback (no event system yet)
            if (m_Callback)
                m_Callback();
        }

        // --- Render ONCE per frame ---
        Render();
    }

    void X11Window::Render()
    {
        if (!m_Display || !m_Window)
            return;

        // Clear old contents (fixes resize artifacts)
        XClearWindow(m_Display, m_Window);

        // Simple background fill
        XSetForeground(m_Display, m_GC, 0x202020);

        XFillRectangle(
            m_Display,
            m_Window,
            m_GC,
            0, 0,
            m_Props.Width,
            m_Props.Height
        );

        // Flush to X server
        XFlush(m_Display);
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
        return (void*)m_Window;
    }

    void X11Window::Shutdown()
    {
        if (m_Display)
        {
            if (m_Window)
                XDestroyWindow(m_Display, m_Window);

            XCloseDisplay(m_Display);

            m_Display = nullptr;
            m_Window = 0;
        }
    }
}