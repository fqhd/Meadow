#pragma once
#include <string>
#include <unordered_map>
#include "Timer.h"

class Benchmarker {
public:

    static void Start(const std::string& name);
    static void End(const std::string& name);

private:

    static std::unordered_map<std::string, Timer> timers;

};