#pragma once
#include <chrono>
#include <optional>

struct MetricsSample {
    std::chrono::system_clock::time_point timestamp;

    int cpu_power_mw;
    int gpu_power_mw;
    int ane_power_mw;
    int combined_power_mw;
};
