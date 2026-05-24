#include "telemetry_dto.hpp"

class ITelemetrySender {
public:
    virtual ~ITelemetrySender() = default;
    
    virtual void sendPacket(const TelemetryDTO& packet) = 0; 
};