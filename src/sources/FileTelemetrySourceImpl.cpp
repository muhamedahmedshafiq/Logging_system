#include <iostream>
#include "sources/FileTelemetrySourceImpl.hpp"


FileTelemetrySourceImpl::FileTelemetrySourceImpl(const std::string& path) : filePath{path}, filePtr{nullptr} {
    
}

bool FileTelemetrySourceImpl::openSource() {
    // We instantiate the RAII SafeFile here
    filePtr = std::make_unique<SafeFile>(filePath);
    
    // Check if it actually opened successfully
    if (!filePtr->IsOpen()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }
    return true;
}

bool FileTelemetrySourceImpl::readSource(std::string& out) {
    if (filePtr && filePtr->IsOpen()) {
        out = filePtr->Read();
        return true;
    }
    return false; // Could not read because source isn't open
}