#include "Time.hpp"
#include <chrono>

namespace Raycaster
{
    using clock = std::chrono::high_resolution_clock;

    Time::Time()
        : deltaTime(0.0f),
          elapsedTime(0.0f),
          timeScale(1.0f),
          m_LastTime(clock::now())
    {
    }

    void Time::beginFrame()
    {
        auto current = clock::now();

        std::chrono::duration<float> diff = current - m_LastTime;
        deltaTime = diff.count();

        m_LastTime = current;

        elapsedTime += deltaTime;
    }

    void Time::endFrame()
    {
    }

    float Time::getDeltaTime() const
    {
        return deltaTime;
    }

    float Time::getScaledDeltaTime() const
    {
        return deltaTime * timeScale;
    }

    float Time::getElapsedTime() const
    {
        return elapsedTime;
    }

    void Time::setTimeScale(float scale)
    {
        timeScale = (scale < 0.0f) ? 0.0f : scale;
    }
}