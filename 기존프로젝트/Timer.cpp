#include "stdafx.h"
#include "Timer.h"


void CTimer::Start()
{
    stop = false;
    lastTick = std::chrono::steady_clock::now();
}


void CTimer::Tick()
{
    if (stop) {
        timeElapsed = 0.0f;
        return;
    }

    auto now = std::chrono::steady_clock::now();
    timeElapsed = std::chrono::duration<float>(now - lastTick).count();
    lastTick = now;
}