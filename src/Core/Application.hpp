#pragma once

#include <memory>

#include "Core/Window.h"

namespace Axiom
{
    class Application
    {
    public:
        Application();
        virtual ~Application() = default;

        void run();
        void close() { m_isRunning = false; }

    private:
        bool m_isRunning = true;
        std::unique_ptr<Window> m_Window;
    };
}