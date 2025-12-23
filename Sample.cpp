#include "Sample.hpp"

MetricsSample::MetricsSample() = default;

int MetricsSample::getCPUCount() const {
    return static_cast<int>(cpu_frequency.size());
}

std::chrono::system_clock::time_point MetricsSample::getTimestamp() const {
    return timestamp;
}

int MetricsSample::getCpuPowerMw() const {
    return cpu_power_mw;
}

int MetricsSample::getGpuPowerMw() const {
    return gpu_power_mw;
}

int MetricsSample::getAnePowerMw() const {
    return ane_power_mw;
}

int MetricsSample::getCombinedPowerMw() const {
    return combined_power_mw;
}

int MetricsSample::getCpuFrequency(int core) const {
    auto it = cpu_frequency.find(core);
    if (it != cpu_frequency.end()) {
        return it->second;
    }
    return 0;
}

double MetricsSample::getCpuActiveResidence(int core) const {
    auto it = cpu_active_residence.find(core);
    if (it != cpu_active_residence.end()) {
        return it->second;
    }
    return 0.0;
}

void MetricsSample::setTimestamp(const std::chrono::system_clock::time_point& ts) {
    timestamp = ts;
}

void MetricsSample::setCpuPowerMw(int power) {
    cpu_power_mw = power;
}

void MetricsSample::setGpuPowerMw(int power) {
    gpu_power_mw = power;
}

void MetricsSample::setAnePowerMw(int power) {
    ane_power_mw = power;
}

void MetricsSample::setCombinedPowerMw(int power) {
    combined_power_mw = power;
}

void MetricsSample::setCpuFrequency(int core, int frequency) {
    cpu_frequency[core] = frequency;
}

void MetricsSample::setCpuActiveResidence(int core, double residence) {
    cpu_active_residence[core] = residence;
}

std::ostream& operator<<(std::ostream& os, const MetricsSample& sample) {
    os << "Timestamp: " << std::chrono::system_clock::to_time_t(sample.timestamp)
       << ", CPU Power: " << sample.getCpuPowerMw() << " mW"
       << ", GPU Power: " << sample.getGpuPowerMw() << " mW"
       << ", ANE Power: " << sample.getAnePowerMw() << " mW"
       << ", Combined Power: " << sample.getCombinedPowerMw() << " mW"
       << ", CPU Frequencies: {";
    for (const auto& [core, freq] : sample.cpu_frequency) {
        os << "Core " << core << ": " << freq << " MHz, ";
    }
    os << "}, CPU Active Residences: {";
    for (const auto& [core, residence] : sample.cpu_active_residence) {
        os << "Core " << core << ": " << residence << " %, ";
    }
    os << "}";
    return os;
}