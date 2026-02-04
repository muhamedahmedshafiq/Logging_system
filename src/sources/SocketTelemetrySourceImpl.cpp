#include <iostream>
#include "sources/SocketTelemetrySourceImpl.hpp"


SocketTelemetrySourceImpl::SocketTelemetrySourceImpl(const std::string& path) : filePath{path}, socketPtr{nullptr} {
    
}

bool SocketTelemetrySourceImpl::openSource() {
    // We instantiate the RAII SafeFile here
    socketPtr = std::make_unique<SafeSocket>(filePath);
    
    // Check if it actually opened successfully
    return (socketPtr && socketPtr->IsOpen());
}

bool SocketTelemetrySourceImpl::readSource(std::string& out) {
    if (socketPtr && socketPtr->IsOpen()) {
        out = socketPtr->Read();
        return true;
    }
    return false; // Could not read because source isn't open
}