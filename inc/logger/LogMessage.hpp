#pragma once

#include <string>
#include <chrono>
#include <ostream>

enum class LogType {
    INFO,
    WARNING,
    ERROR
};

class LogMessage {
private:
    std::string app_name;
    std::string context;
    std::chrono::time_point<std::chrono::system_clock> timestamp;
    LogType severity;
    std::string message;

public:
    LogMessage(const std::string& app_name,
               const std::string& context,
               LogType severity,
               std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now(),
               const std::string& message = "");

    friend std::ostream& operator<<(std::ostream& os, const LogMessage& msg);

    ~LogMessage() = default;
};
