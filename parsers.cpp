#include "parsers.hpp"

bool TimeStampParser::parse(const std::string& line, MetricsSample& sample) const {
    if (line.rfind("*** Sampled system activity", 0) != 0)
        return false;

    static const RE2 re(
        R"(Sampled system activity \(([^)]+)\))");

    std::string ts;
    if (!RE2::PartialMatch(line, re, &ts))
        return false;

    std::tm tm{};
    constexpr const char* format = "%a %b %d %H:%M:%S %Y %z";

    if (!strptime(ts.c_str(), format, &tm))
        return false;

    time_t t = mktime(&tm);
    if (t == static_cast<time_t>(-1))
        return false;

    sample.timestamp = std::chrono::system_clock::from_time_t(t);
    return true;
}

bool CpuPowerParser::parse(const std::string& line, MetricsSample& sample) const {
    static const RE2 re(R"(CPU Power:\s+(\d+)\s+mW)");
    int value = 0;
    if (RE2::PartialMatch(line, re, &value)) {
        sample.cpu_power_mw = value;
        return true;
    }
    return false;
}

bool GpuPowerParser::parse(const std::string& line, MetricsSample& sample) const {
    static const RE2 re(R"(GPU Power:\s+(\d+)\s+mW)");
    int value = 0;
    if (RE2::PartialMatch(line, re, &value)) {
        sample.gpu_power_mw = value;
        return true;
    }
    return false;
}

bool AnePowerParser::parse(const std::string& line, MetricsSample& sample) const {
    static const RE2 re(R"(ANE Power:\s+(\d+)\s+mW)");
    int value = 0;
    if (RE2::PartialMatch(line, re, &value)) {
        sample.ane_power_mw = value;
        return true;
    }
    return false;
}

bool CombinedPowerParser::parse(const std::string& line, MetricsSample& sample) const {
    static const RE2 re(R"(Combined Power (CPU + GPU + ANE):\s+(\d+)\s+mW)");
    int value = 0;
    if (RE2::PartialMatch(line, re, &value)) {
        sample.combined_power_mw = value;
        return true;
    }
    return false;
}