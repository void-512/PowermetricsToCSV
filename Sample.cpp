#include "Sample.hpp"

MetricsSample::MetricsSample() = default;

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

std::ostream& operator<<(std::ostream& os, const MetricsSample& sample) {
    os << "Timestamp: " << std::chrono::system_clock::to_time_t(sample.timestamp)
       << ", CPU Power: " << sample.getCpuPowerMw() << " mW"
       << ", GPU Power: " << sample.getGpuPowerMw() << " mW"
       << ", ANE Power: " << sample.getAnePowerMw() << " mW"
       << ", Combined Power: " << sample.getCombinedPowerMw() << " mW";
    return os;
}