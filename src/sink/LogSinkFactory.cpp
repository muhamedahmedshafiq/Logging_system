#include "sink/LogSinkFactory.hpp"
#include "sink/ConsoleSinkImpl.hpp"
#include "sink/FileSinkImpl.hpp"


std::unique_ptr<ILogSink> LogSinkFactory::createSink(LogSinkType_enum type, 
                                                    const std::string& filePath) {
    switch (type) {
        case LogSinkType_enum::CONSOLE:
            return std::make_unique<ConsoleSink>();

        case LogSinkType_enum::FILE:
            // Uses "system.log" if no path is provided
            return std::make_unique<FileSink>(filePath.empty() ? "system.log" : filePath);

        default:
            // If someone passes an invalid enum value
            return nullptr;
    }
}