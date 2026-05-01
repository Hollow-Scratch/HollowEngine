#pragma once

#include <memory>
#include "Core/Time.hpp"

namespace Hollow {

class Window;

class Application {
public:
    Application();
    virtual ~Application() noexcept;

    void Run();

    static Application& Get() { return *s_Instance; }

    Window& GetWindow() { return *m_Window; }
    Time& GetTime() { return m_Time; }

protected:
    virtual void OnInit() {}
    virtual void OnUpdate(float dt) {}
    virtual void OnShutdown() {}

protected:
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;

private:
    static Application* s_Instance;

    Time m_Time;
};

}