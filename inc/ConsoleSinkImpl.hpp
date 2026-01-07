#pragma once

#include "ILogSink.hpp"



class ConsoleSink : public ILogSink {
public:
    void write(const LogMessage& message) override;
};

