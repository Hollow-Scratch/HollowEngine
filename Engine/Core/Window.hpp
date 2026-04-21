#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>

namespace Axiom {
struct WindowProps {
  std::string Title = "Game";
  std::uint32_t Width = 1280;
  std::uint32_t Height = 720;
};

class Window {
public:
  using EventCallbackFn = std::function<void()>;

  virtual ~Window() = default;

  virtual void OnUpdate() = 0;

  virtual std::uint32_t GetWidth() const = 0;
  virtual std::uint32_t GetHeight() const = 0;
  virtual bool ShouldClose() const = 0;

  virtual void SetEventCallback(const EventCallbackFn &callback) = 0;

  virtual void SetVSync(bool enabled) = 0;
  virtual bool IsVSync() const = 0;

  virtual void *GetNativeWindow() const = 0;

  static std::unique_ptr<Window>
  Create(const WindowProps &props = WindowProps());
};
}
