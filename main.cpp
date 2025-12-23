#include <iostream>
#include "Parsers.hpp"
#include "Sample.hpp"
#include "FileReader.hpp"

void exportToCsv(const std::vector<MetricsSample>& parsedData,
                 const std::string& filePath)
{
    if (parsedData.empty()) {
        throw std::runtime_error("No data to export");
    }

    std::ofstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open CSV file");
    }

    const int cpuCount = parsedData.front().getCPUCount();

    /* ---------- Header ---------- */
    file << "Timestamp,"
         << "CPU Power (mW),"
         << "GPU Power (mW),"
         << "ANE Power (mW),"
         << "Combined Power (mW)";

    for (int core = 0; core < cpuCount; ++core) {
        file << ",CPU" << core << " Frequency (MHz)"
             << ",CPU" << core << " Active (%)";
    }
    file << "\n";

    /* ---------- Rows ---------- */
    for (const auto& sample : parsedData) {
        file << std::chrono::system_clock::to_time_t(sample.getTimestamp()) << ","
             << sample.getCpuPowerMw() << ","
             << sample.getGpuPowerMw() << ","
             << sample.getAnePowerMw() << ","
             << sample.getCombinedPowerMw();

        for (int core = 0; core < cpuCount; ++core) {
            file << "," << sample.getCpuFrequency(core)
                 << "," << sample.getCpuActiveResidence(core);
        }
        file << "\n";
    }

    file.close();
}


int main(int argc, char* argv[]) {
    FileReader fileReader("sample.txt");
    std::vector<MetricsSample> parsedData;
    ParseTask parseTask(parsedData);
    fileReader.start();
    std::vector<std::string> task;
    while (fileReader.getTask(task)) {
        parseTask.parse(task);
    }
    exportToCsv(parsedData, "output.csv");
    return 0;
}
