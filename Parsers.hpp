#ifndef __PARSERS_HPP__
#define __PARSERS_HPP__
#include "Sample.hpp"
#include <string>
#include <vector>
#include <memory>

enum class ParserType {
    Timestamp,
    CpuPower,
    GpuPower,
    AnePower,
    CombinedPower,
    CPUFrequency,
    CPUActiveResidence,
    Count
};

class LineParser {
public:
    virtual ~LineParser() = default;
    virtual bool parse(const std::string& line, MetricsSample& sample) const = 0;
    virtual ParserType type() const = 0;
};

class TimeStampParser final : public LineParser {
public:
    bool parse(const std::string& line, MetricsSample& sample) const override;
    ParserType type() const override;
};

class CpuPowerParser final : public LineParser {
public:
    bool parse(const std::string& line, MetricsSample& sample) const override;
    ParserType type() const override;
};

class GpuPowerParser final : public LineParser {
public:
    bool parse(const std::string& line, MetricsSample& sample) const override;
    ParserType type() const override;
};

class AnePowerParser final : public LineParser {
public:
    bool parse(const std::string& line, MetricsSample& sample) const override;
    ParserType type() const override;
};

class CombinedPowerParser final : public LineParser {
public:
    bool parse(const std::string& line, MetricsSample& sample) const override;
    ParserType type() const override;
};

class CPUFrequencyParser final : public LineParser {
public:
    bool parse(const std::string& line, MetricsSample& sample) const override;
    ParserType type() const override;
};

class CPUActiveResidenceParser final : public LineParser {
public:
    bool parse(const std::string& line, MetricsSample& sample) const override;
    ParserType type() const override;
};

class ParseTask {
    std::vector<std::unique_ptr<LineParser>> parsers;
    std::vector<MetricsSample>& data;
public:
    ParseTask(std::vector<MetricsSample>& d);
    void parse(std::vector<std::string>& task);
};

std::vector<std::unique_ptr<LineParser>> makeParsers();

#endif