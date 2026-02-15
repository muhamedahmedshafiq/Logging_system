#pragma once

#include <string>
#include <optional>
#include <chrono>
#include "logger/LogMessage.hpp"
#include "enums/SeverityLevel.hpp"

// Bridge between Policy Severity and LogMessage Type
inline LogType mapToLogType(SeverityLvl_enum level) {
    switch (level) {
        case SeverityLvl_enum::CRITICAL: return LogType::ERROR;
        case SeverityLvl_enum::WARNING:  return LogType::WARNING;
        default:                         return LogType::INFO;
    }
}

template <typename Policy>
class LogFormatter {
private:
    std::string app_name;
    std::string context;

    // Helper for message description
    std::string msgDescription(float val) const;

public:
    LogFormatter(std::string app, std::string ctx);

    //  Main formatting 
    std::optional<LogMessage> formatDataToLogMsg(const std::string& rawData);
};

// CRITICAL: Templates must have their implementation visible to the header
#include "LogFormatter.tpp"
