#pragma once
#include <chrono>

class Timer {
public:

    void Start();
    float End();

private:
    std::chrono::high_resolution_clock::time_point start;

};