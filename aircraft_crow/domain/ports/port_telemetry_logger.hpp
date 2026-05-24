#pragma once
#include "telemetry_dto.hpp"

class ITelemetryLogger {
public:
    virtual ~ITelemetryLogger() = default;

    virtual void openLog() = 0;
    
    virtual void logRawPacket(const TelemetryDTO& packet) = 0;
    
    virtual void closeLog() = 0; 
};