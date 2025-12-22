#ifndef __FILE_READER_HPP__
#define __FILE_READER_HPP__
#include <queue>
#include <vector>
#include <string>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>

class FileReader {
    std::queue<std::vector<std::string>> taskQueue;
    std::mutex mtx;
    std::condition_variable cv;
    std::thread readerThread;
    std::string filePath;
    bool finished = false;
    void readLoop();
    bool isSampleHeader(const std::string& line) const;

public:
    explicit FileReader(std::string filePath);
    ~FileReader();

    bool start();
    bool getTask(std::vector<std::string>& task);
};

#endif