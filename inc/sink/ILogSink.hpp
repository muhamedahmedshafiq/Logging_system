#pragma once
#include "logger/LogMessage.hpp"


//BaseSink

class ILogSink { 
public:
    virtual void write(const LogMessage& message) = 0;
    virtual ~ILogSink() = default;
};