#pragma once

#include <memory>

namespace Raycaster {

class Window;

class Application {
public:
    Application();
    virtual ~Application() noexcept;

    void Run();

    static Application& Get() { return *s_Instance; }

    Window& GetWindow() { return *m_Window; }

protected:
    virtual void OnInit() {}
    virtual void OnUpdate(float dt) {}
    virtual void OnShutdown() {}

protected:
    std::unique_ptr<Window> m_Window;
    bool m_Running = true;

private:
    static Application* s_Instance;
};

}