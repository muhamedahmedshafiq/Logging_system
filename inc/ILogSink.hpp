#pragma once
#include "LogMessage.hpp"

class ILogSink { 
public:
    virtual void write(const LogMessage& message) = 0;
    virtual ~ILogSink() = default;
};