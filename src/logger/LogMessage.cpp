#include "logger/LogMessage.hpp"
#include <iomanip>
#include <ctime>

LogMessage::LogMessage(const std::string& app_name,
                       const std::string& context,
                       LogType severity,
                       std::chrono::system_clock::time_point timestamp,
                       const std::string& message)
    : app_name{app_name}, context{context}, timestamp{timestamp}, 
      severity{severity}, message{message} {}


      
std::string LogTypeToString(LogType type) {
    switch (type) {
        case LogType::INFO:    return "INFO";
        case LogType::WARNING: return "WARNING";
        case LogType::ERROR:   return "ERROR";
        default:               return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& os, const LogMessage& msg) {

    std::time_t t = std::chrono::system_clock::to_time_t(msg.timestamp);
    std::tm tm = *std::localtime(&t);

    os << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " "
       << "[" << std::left << std::setw(7) << LogTypeToString(msg.severity) << "] "
       << "[" << msg.app_name << "::" << msg.context << "] "
       << msg.message;
       
    return os;
}