#pragma once

#include <vector>
#include "LogMessage.hpp"
#include "sink/ILogSink.hpp"
#include <memory>

class LogManager{
private:
    std::vector<LogMessage> logs;
    std::vector<std::unique_ptr<ILogSink>> sinks;
public:
    LogManager() = default;
    ~LogManager() = default;
    // Delete copy operations (unique_ptr cannot be copied)
    LogManager(const LogManager&) = delete;
    LogManager& operator=(const LogManager&) = delete;
    // Allow move operations
    LogManager(LogManager&&) = default;
    LogManager& operator=(LogManager&&) = default;
    
    void addLog(const LogMessage& log);
    void addSink(std::unique_ptr<ILogSink> sink);
    void routeLogsForAllSinks();

};