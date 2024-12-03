#pragma once
#include "Common.h"
#include <chrono>

class CTimer
{
public:
    CTimer() : stop(false), timeElapsed(0) {}

    void Start();

    void Stop() { stop = true; }

    // �����Ӹ��� ȣ���Ͽ� lastTick �� timeElapsed ����
    void Tick();

    auto Now() { return std::chrono::steady_clock::now(); }

    float GetElapsedTime() const { return timeElapsed; }

private:
    bool stop;
    float timeElapsed;
    std::chrono::steady_clock::time_point lastTick;
};

