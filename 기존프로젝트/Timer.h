#pragma once
#include <chrono>
#include <functional>

class CTimer
{
public:
    CTimer() : stop(false), timeElapsed(0) {}

    void Start();

    void Stop() {stop = true; }

    // 프레임마다 호출하여 lastTick 및 timeElapsed 갱신
    void Tick();

    float GetElapsedTime() const { return timeElapsed; }

private:
    bool stop;
    float timeElapsed;
    std::chrono::steady_clock::time_point lastTick;
};

