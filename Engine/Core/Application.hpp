#pragma once

#include <memory>

#include "Core/Window.hpp"

namespace Axiom {
class Application {
public:
  Application();
  virtual ~Application() = default;

  void run();
  void close() { m_isRunning = false; }

protected:
  virtual void OnInit() {}
  virtual void OnUpdate(float deltaTime) {}
  virtual void OnShutdown() {}

private:
  bool m_isRunning = true;
  std::unique_ptr<Window> m_Window;
};

  Application* CreateApplication();
}
