#pragma once

namespace Axiom
{
    class Time
    {
    public:
        static void Reset();
        static void Update();

        static float GetDeltaTime() { return s_DeltaTime; }
        static float GetTime() { return s_Time; }

    private:
        static float s_Time;
        static float s_DeltaTime;
    };
}