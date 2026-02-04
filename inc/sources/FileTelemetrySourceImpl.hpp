#pragma once

#include <string>
#include "ITelemetrySource.hpp"
#include "raii/SafeFile.hpp"
#include <memory>

class FileTelemetrySourceImpl: public ITelemetrySource{
    private:
        std::string filePath;
        std::unique_ptr<SafeFile> filePtr;
    public:
        explicit FileTelemetrySourceImpl(const std::string& path);
        bool openSource() override;
        bool readSource(std::string& out) override;

       ~FileTelemetrySourceImpl() override = default;
};
