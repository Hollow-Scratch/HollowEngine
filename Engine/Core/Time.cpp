#include "Time.hpp"
#include <chrono>

namespace Axiom
{
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;

    static TimePoint s_StartTime;
    static TimePoint s_LastFrameTime;

    float Time::s_Time = 0.0f;
    float Time::s_DeltaTime = 0.0f;

    void Time::Reset()
    {
        s_StartTime = Clock::now();
        s_LastFrameTime = s_StartTime;
        s_Time = 0.0f;
        s_DeltaTime = 0.0f;
    }

    void Time::Update()
    {
        TimePoint now = Clock::now();

        std::chrono::duration<float> delta = now - s_LastFrameTime;
        s_DeltaTime = delta.count();

        std::chrono::duration<float> total = now - s_StartTime;
        s_Time = total.count();

        s_LastFrameTime = now;

        if (s_DeltaTime > 0.1f)
            s_DeltaTime = 0.1f;
    }
}