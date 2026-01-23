#pragma once

#include <string>
#include "ITelemetrySource.hpp"
#include "SafeSocket.hpp"
#include <memory>


class SocketTelemetrySourceImpl: public ITelemetrySource{
    private:
    std::string filePath;
    std::unique_ptr<SafeSocket> socketPtr;
    public:
        explicit SocketTelemetrySourceImpl(const std::string &path);
        bool openSource() override;
        bool readSource(std::string& out) override;
        ~SocketTelemetrySourceImpl()=default;

};