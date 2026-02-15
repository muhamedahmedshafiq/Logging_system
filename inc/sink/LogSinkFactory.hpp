#pragma once

#include <memory>
#include <string>
#include "enums/LogSinkType.hpp"
#include "ILogSink.hpp"

class LogSinkFactory {
public:

    static std::unique_ptr<ILogSink> createSink(LogSinkType_enum type, 
                                                const std::string& filePath = "");
};