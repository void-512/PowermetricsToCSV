#ifndef __SAMPLE_HPP__
#define __SAMPLE_HPP__
#include <chrono>
#include <ostream>
#include <unordered_map>

class MetricsSample {
    std::chrono::system_clock::time_point timestamp;

    int cpu_power_mw;
    int gpu_power_mw;
    int ane_power_mw;
    int combined_power_mw;
    std::unordered_map<int, int> cpu_frequency;
    std::unordered_map<int, double> cpu_active_residence;
public:
    MetricsSample();
    int getCPUCount() const;

    std::chrono::system_clock::time_point getTimestamp() const;
    int getCpuPowerMw() const;
    int getGpuPowerMw() const;
    int getAnePowerMw() const;
    int getCombinedPowerMw() const;
    int getCpuFrequency(int core) const;
    double getCpuActiveResidence(int core) const;

    void setTimestamp(const std::chrono::system_clock::time_point& ts);
    void setCpuPowerMw(int power);
    void setGpuPowerMw(int power);
    void setAnePowerMw(int power);
    void setCombinedPowerMw(int power);
    void setCpuFrequency(int core, int frequency);
    void setCpuActiveResidence(int core, double residence);

    friend std::ostream& operator<<(std::ostream& os, const MetricsSample& sample);
};

std::ostream& operator<<(std::ostream& os, const MetricsSample& sample);
#endif