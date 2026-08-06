#include "Benchmarker.h"
#include <iostream>

std::unordered_map<std::string, Timer> Benchmarker::timers;

void Benchmarker::Start(const std::string& name)
{
    timers[name] = Timer();
    timers[name].Start();
}

void Benchmarker::End(const std::string& name)
{
    auto timer = timers.find(name);
    if(timer != timers.end()){
        std::cout << name << " Time: " << timer->second.End() << std::endl;
    }else{
        std::cout << "Couldn't find timer with name: " << name << std::endl;
    }
}