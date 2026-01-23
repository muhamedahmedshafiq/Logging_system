#include <iostream>
#include "FileTelemetrySourceImpl.hpp"


FileTelemetrySourceImpl::FileTelemetrySourceImpl(const std::string& path) : filePath{path}, filePtr{nullptr} {
    
}

bool FileTelemetrySourceImpl::openSource() {
    // We instantiate the RAII SafeFile here
    filePtr = std::make_unique<SafeFile>(filePath);
    
    // Check if it actually opened successfully
    return (filePtr && filePtr->IsOpen());
}

bool FileTelemetrySourceImpl::readSource(std::string& out) {
    if (filePtr && filePtr->IsOpen()) {
        out = filePtr->Read();
        return true;
    }
    return false; // Could not read because source isn't open
}