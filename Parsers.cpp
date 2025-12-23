#include "Parsers.hpp"
#include <ctime>
#include <chrono>
#include <bitset>
#include <iostream>
#include <re2/re2.h>

std::vector<std::unique_ptr<LineParser>> makeParsers() {
    std::vector<std::unique_ptr<LineParser>> parsers;
    parsers.emplace_back(std::make_unique<TimeStampParser>());
    parsers.emplace_back(std::make_unique<CpuPowerParser>());
    parsers.emplace_back(std::make_unique<GpuPowerParser>());
    parsers.emplace_back(std::make_unique<AnePowerParser>());
    parsers.emplace_back(std::make_unique<CombinedPowerParser>());
    parsers.emplace_back(std::make_unique<CPUFrequencyParser>());
    parsers.emplace_back(std::make_unique<CPUActiveResidenceParser>());
    return parsers;
}

ParserType TimeStampParser::type() const {
    return ParserType::Timestamp;
}

ParserType CpuPowerParser::type() const {
    return ParserType::CpuPower;
}

ParserType GpuPowerParser::type() const {
    return ParserType::GpuPower;
}

ParserType AnePowerParser::type() const {
    return ParserType::AnePower;
}

ParserType CombinedPowerParser::type() const {
    return ParserType::CombinedPower;
}

ParserType CPUFrequencyParser::type() const {
    return ParserType::CPUFrequency;
}

ParserType CPUActiveResidenceParser::type() const {
    return ParserType::CPUActiveResidence;
}

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

    sample.setTimestamp(std::chrono::system_clock::from_time_t(t));
    return true;
}

bool CpuPowerParser::parse(const std::string& line, MetricsSample& sample) const {
    static const RE2 re(R"(CPU Power:\s+(\d+)\s+mW)");
    int value = 0;
    if (RE2::PartialMatch(line, re, &value)) {
        sample.setCpuPowerMw(value);
        return true;
    }
    return false;
}

bool GpuPowerParser::parse(const std::string& line, MetricsSample& sample) const {
    static const RE2 re(R"(GPU Power:\s+(\d+)\s+mW)");
    int value = 0;
    if (RE2::PartialMatch(line, re, &value)) {
        sample.setGpuPowerMw(value);
        return true;
    }
    return false;
}

bool AnePowerParser::parse(const std::string& line, MetricsSample& sample) const {
    static const RE2 re(R"(ANE Power:\s+(\d+)\s+mW)");
    int value = 0;
    if (RE2::PartialMatch(line, re, &value)) {
        sample.setAnePowerMw(value);
        return true;
    }
    return false;
}

bool CombinedPowerParser::parse(const std::string& line, MetricsSample& sample) const {
    static const RE2 re(R"(Combined Power \(CPU \+ GPU \+ ANE\):\s+(\d+)\s+mW)");
    int value = 0;
    if (RE2::PartialMatch(line, re, &value)) {
        sample.setCombinedPowerMw(value);
        return true;
    }
    return false;
}

bool CPUFrequencyParser::parse(const std::string& line, MetricsSample& sample) const {
        static const RE2 re(
            R"(CPU\s+(\d+)\s+frequency:\s+(\d+)\s+MHz)"
        );

        int cpu = 0;
        int freq = 0;

        if (RE2::PartialMatch(line, re, &cpu, &freq)) {
            sample.setCpuFrequency(cpu, freq);
            return true;
        }
        return false;
    }

bool CPUActiveResidenceParser::parse(const std::string& line, MetricsSample& sample) const {
    static const RE2 re(
        R"(CPU\s+(\d+)\s+active\sresidency:\s*([\d.]+)%)"
    );

    int cpu = 0;
    double util = 0.0;

    if (RE2::PartialMatch(line, re, &cpu, &util)) {
        sample.setCpuActiveResidence(cpu, util);
        return true;
    }
    return false;
}

ParseTask::ParseTask(std::vector<MetricsSample>& d): parsers{makeParsers()}, data{d} {}

void ParseTask::parse(std::vector<std::string>& task) {
    MetricsSample sample{};
    std::bitset<static_cast<size_t>(ParserType::Count)> seen;
    for (const auto& line : task) {
        for (const auto& parser : parsers) {
            if (parser->parse(line, sample)) {
                seen.set(static_cast<size_t>(parser->type()));
            }
        }
    }
    if (seen.all()) {
        data.push_back(sample);
    }
}