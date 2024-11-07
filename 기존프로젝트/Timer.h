#pragma once
#include <chrono>
#include <functional>

class CTimer
{
public:
    CTimer() : stop(false), timeElapsed(0) {}

    void Start();

    void Stop() {stop = true; }

    // �����Ӹ��� ȣ���Ͽ� lastTick �� timeElapsed ����
    void Tick();

    float GetElapsedTime() const { return timeElapsed; }

private:
    bool stop;
    float timeElapsed;
    std::chrono::steady_clock::time_point lastTick;
};

