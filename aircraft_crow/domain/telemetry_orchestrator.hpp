#pragma once
#include "esp_log.h"

#include "system_state.hpp"

#include "port_telemetry_sender.hpp"
#include "port_telemetry_logger.hpp"
#include "port_system_indicator.hpp"
#include "port_sensor.hpp"

#include "telemetry_dto.hpp"


class TelemetryOrchestrator {
private:
    SystemState _currentState;
    ITelemetrySender* _sender;
    ITelemetryLogger* _logger;
    ISystemIndicator* _indicator;
    IPilotReceiver* _pilot_receiver;

    TelemetryDTO quantize(const TelemetryDTO& rawPacket);

public:
    TelemetryOrchestrator(ITelemetrySender* sender, ITelemetryLogger* logger, ISystemIndicator* indicator, IPilotReceiver* pilot_receiver)
        : _currentState(SystemState::RECORDING), _sender(sender), _logger(logger), _indicator(indicator), _pilot_receiver(pilot_receiver) {}

    void setRecordingMode(bool isRecording);

    void processSensorData(const TelemetryDTO& rawPacket);
};