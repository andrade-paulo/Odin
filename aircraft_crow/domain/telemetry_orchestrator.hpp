#pragma once
#include "esp_log.h"

#include "system_state.hpp"

#include "port_telemetry_sender.hpp"
#include "port_telemetry_logger.hpp"
#include "port_system_indicator.hpp"
#include "port_imu.hpp"
#include "port_barometer.hpp"
#include "port_gps.hpp"

#include "telemetry_dto.hpp"


class TelemetryOrchestrator {
private:
    SystemState _currentState;
    ITelemetrySender* _sender;
    ITelemetryLogger* _logger;
    ISystemIndicator* _indicator;
    IImuSensor* _imu;
    IBarometerSensor* _barometer;
    IGpsSensor* _gps;

    TelemetryDTO quantize(const TelemetryDTO& rawPacket);

public:
    TelemetryOrchestrator(ITelemetrySender* sender, ITelemetryLogger* logger, ISystemIndicator* indicator)
        : _currentState(SystemState::RECORDING), _sender(sender), _logger(logger), _indicator(indicator) {}

    void setRecordingMode(bool isRecording);

    void processSensorData(const TelemetryDTO& rawPacket);

    void setSensorControls(IImuSensor* imu, IBarometerSensor* baro, IGpsSensor* gps) {
        _imu = imu;
        _barometer = baro;
        _gps = gps;
    }
};