#pragma once
#include "parsers.hpp"
#include <vector>

std::vector<std::unique_ptr<LineParser>> makeParsers() {
    std::vector<std::unique_ptr<LineParser>> parsers;
    parsers.emplace_back(std::make_unique<TimeStampParser>());
    parsers.emplace_back(std::make_unique<CpuPowerParser>());
    parsers.emplace_back(std::make_unique<GpuPowerParser>());
    parsers.emplace_back(std::make_unique<AnePowerParser>());
    parsers.emplace_back(std::make_unique<CombinedPowerParser>());
    return parsers;
}
