#include <iostream>
#include "Parsers.hpp"
#include "Sample.hpp"
#include "FileReader.hpp"

void exportToCsv(const std::vector<MetricsSample>& parsedData,
                 const std::string& filePath)
{
    std::ofstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open CSV file");
    }

    file << "Timestamp,CPU Power (mW),GPU Power (mW),ANE Power (mW),Combined Power (mW)\n";
    for (const auto& sample : parsedData) {
        file << std::chrono::system_clock::to_time_t(sample.getTimestamp()) << ","
             << sample.getCpuPowerMw() << ","
             << sample.getGpuPowerMw() << ","
             << sample.getAnePowerMw() << ","
             << sample.getCombinedPowerMw() << "\n";
    }

    file.close();
}


int main(int argc, char* argv[]) {
    FileReader fileReader("singlesample.txt");
    std::vector<MetricsSample> parsedData;
    ParseTask parseTask(parsedData);
    fileReader.start();
    std::vector<std::string> task;
    while (fileReader.getTask(task)) {
        parseTask.parse(task);
        std::cout << parsedData.back() << std::endl;
    }
    exportToCsv(parsedData, "output.csv");
    return 0;
}
