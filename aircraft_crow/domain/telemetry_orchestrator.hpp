#pragma once
#include "system_state.hpp"
#include "port_telemetry_sender.hpp"
#include "port_telemetry_logger.hpp"
#include "port_system_indicator.hpp"
#include "telemetry_dto.hpp"

class TelemetryOrchestrator {
private:
    SystemState _currentState;
    ITelemetrySender* _sender;
    ITelemetryLogger* _logger;
    ISystemIndicator* _indicator;

    TelemetryDTO applyFiltersAndQuantize(const TelemetryDTO& rawPacket);

public:
    TelemetryOrchestrator(ITelemetrySender* sender, ITelemetryLogger* logger, ISystemIndicator* indicator) 
        : _currentState(SystemState::POWER_UP), _sender(sender), _logger(logger), _indicator(indicator) {}

    void finishCalibration();

    void setRecordingMode(bool isRecording);

    void processSensorData(const TelemetryDTO& rawPacket);
};