#include <iostream>
#include <memory>
#include <vector>

// Phase 1 Includes
#include "LogManager.hpp"
#include "LogMessage.hpp"
#include "ConsoleSinkImpl.hpp"
#include "FileSinkImpl.hpp"

// Phase 2 Includes (The new stuff!)
#include "FileTelemetrySourceImpl.hpp"
#include "SocketTelemetrySourceImpl.hpp"

int main() {
    std::cout << "--- Phase 2: RAII & Source Management Test ---" << std::endl;

    // 1. Setup Sinks from Phase 1
    LogManager logMang;
    logMang.addSink(std::make_unique<ConsoleSink>());

    // 2. Setup Sources from Phase 2
    // Testing File Source (reading from a system file like CPU info)
    auto fileSource = std::make_unique<FileTelemetrySourceImpl>("/proc/loadavg");
    
    // Testing Socket Source (ensure your mock server is running or this will fail)
    auto socketSource = std::make_unique<SocketTelemetrySourceImpl>("/tmp/telemetry.sock");

    // 3. Demonstrate RAII Resource Opening
    std::vector<std::unique_ptr<ITelemetrySource>> sources;
    sources.push_back(std::move(fileSource));
    sources.push_back(std::move(socketSource));

    for (auto& source : sources) {
        if (source->openSource()) {
            std::string rawData;
            if (source->readSource(rawData)) {
                // For Phase 2, we manually create the message to prove we read the data
                LogMessage msg("TelemetrySystem", 
               "SourceScanner", 
               LogType::INFO, 
               std::chrono::system_clock::now(), // Add this!
               "Raw Data Read: " + rawData);
                
                logMang.addLog(std::move(msg));
            }
        } else {
            std::cout << "Warning: Could not open a source!" << std::endl;
        }
    }

    // 4. Route logs
    logMang.routeLogsForAllSinks();

    return 0;
}