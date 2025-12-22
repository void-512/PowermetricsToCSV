#include "FileReader.hpp"

FileReader::FileReader(std::string path): filePath(std::move(path)) {}

FileReader::~FileReader() {
    if (readerThread.joinable())
        readerThread.join();
}

bool FileReader::start() {
    readerThread = std::thread(&FileReader::readLoop, this);
    return true;
}

void FileReader::readLoop() {
    std::ifstream in(filePath);
    if (!in.is_open()) {
        std::lock_guard<std::mutex> lock(mtx);
        finished = true;
        cv.notify_all();
        return;
    }

    std::vector<std::string> current;
    std::string line;

    while (std::getline(in, line)) {
        if (isSampleHeader(line)) {
            if (!current.empty()) {
                {
                    std::lock_guard<std::mutex> lock(mtx);
                    taskQueue.push(std::move(current));
                }
                cv.notify_one();
                current.clear();
            }
        }
        current.push_back(line);
    }

    if (!current.empty()) {
        std::lock_guard<std::mutex> lock(mtx);
        taskQueue.push(std::move(current));
        cv.notify_one();
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        finished = true;
    }
    cv.notify_all();
}

bool FileReader::getTask(std::vector<std::string>& task) {
    std::unique_lock<std::mutex> lock(mtx);

    cv.wait(lock, [&] {
        return !taskQueue.empty() || finished;
    });

    if (taskQueue.empty())
        return false;

    task = std::move(taskQueue.front());
    taskQueue.pop();
    return true;
}

bool FileReader::isSampleHeader(const std::string& line) const {
    return line.rfind("*** Sampled system activity", 0) == 0;
}
