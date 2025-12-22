#pragma once
#include "sample.hpp"
#include <string>

class LineParser {
public:
    virtual ~LineParser() = default;
    virtual bool parse(const std::string& line, MetricsSample& sample) const = 0;
};

class TimeStampParser final : public LineParser {
public:
    bool parse(const std::string& line, MetricsSample& sample) const override;
};

class CpuPowerParser final : public LineParser {
public:
    bool parse(const std::string& line, MetricsSample& sample) const override;
};

class GpuPowerParser final : public LineParser {
public:
    bool parse(const std::string& line, MetricsSample& sample) const override;
};

class AnePowerParser final : public LineParser {
public:
    bool parse(const std::string& line, MetricsSample& sample) const override;
};

class CombinedPowerParser final : public LineParser {
public:
    bool parse(const std::string& line, MetricsSample& sample) const override;
};
