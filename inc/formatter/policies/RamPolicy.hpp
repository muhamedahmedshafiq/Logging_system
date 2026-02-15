//RamPolicy.hpp
#pragma once 

#include "enums/TelemetrySource.hpp"
#include "enums/SeverityLevel.hpp"
#include <string_view>

struct RamPolicy {
    public:
    // Threshold values
    static constexpr std::string_view unit = "%";
    static constexpr float WARN_THRESHOLD = 75.0f;
    static constexpr float CRIT_THRESHOLD = 95.0f;

    static SeverityLvl_enum inferSeverity(float value) {
        if (value >= CRIT_THRESHOLD){
            return SeverityLvl_enum::CRITICAL;
        } 
        if (value >= WARN_THRESHOLD){
            return SeverityLvl_enum::WARNING;
        } 
        return SeverityLvl_enum::INFO;
    }
};