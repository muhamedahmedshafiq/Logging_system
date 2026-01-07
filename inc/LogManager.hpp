#pragma once

#include <vector>
#include "LogMessage.hpp"
#include "ILogSink.hpp"
#include <memory>

class LogManager{
private:
    std::vector<LogMessage> logs;
    std::vector<std::unique_ptr<ILogSink>> sinks;
public:
    LogManager() = default;
    ~LogManager() = default;
    void addLog(const LogMessage& log);
    void addSink(std::unique_ptr<ILogSink> sink);
    void routeLogsForAllSinks();

};