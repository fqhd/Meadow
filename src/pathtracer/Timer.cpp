#include "Timer.h"

void Timer::Start()
{
    start = std::chrono::high_resolution_clock::now();
}

float Timer::End()
{
    std::chrono::duration<float> duration = std::chrono::high_resolution_clock::now() - start;
    return duration.count();
}