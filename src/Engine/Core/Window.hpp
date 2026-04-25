#pragma once

#include <string>
#include <functional>
#include <cstdint>
#include <memory>

namespace Hollow {

struct WindowProps {
    std::string Title = "Axiom Engine";
    uint32_t Width = 1280;
    uint32_t Height = 720;
    bool VSync = true;

    WindowProps(const std::string& title = "Axiom Engine",
                uint32_t width = 1280,
                uint32_t height = 720)
        : Title(title), Width(width), Height(height) {}
};

class Event;
using EventCallbackFn = std::function<void(Event&)>;

class Window {
public:
    virtual ~Window() = default;

    virtual void OnUpdate() = 0;
    virtual void Shutdown() = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;

    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

    virtual void* GetNativeWindow() const = 0;

    virtual bool ShouldClose() const = 0;

    static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());
};

}
