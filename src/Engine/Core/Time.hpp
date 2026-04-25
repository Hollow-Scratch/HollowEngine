#pragma once
#include "Core.hpp"
#include <chrono>

namespace Hollow
{
    class RAYCASTER_API Time
    {
    private:
        float deltaTime;
        float elapsedTime;
        float timeScale;

        std::chrono::high_resolution_clock::time_point m_LastTime;

    public:
        Time();

        void beginFrame();
        void endFrame();

        float getDeltaTime() const;
        float getScaledDeltaTime() const;
        float getElapsedTime() const;

        void setTimeScale(float scale);
    };
}
