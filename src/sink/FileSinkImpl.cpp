#include "sink/FileSinkImpl.hpp"
#include <iostream>

FileSink::FileSink(const std::string& filePath, std::ios::openmode mode) {
    file.open(filePath, mode);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }
}

void FileSink::write(const LogMessage& log){
    file << log << std::endl;
}

