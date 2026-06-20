#include "telemetry_orchestrator.hpp"

static const char* TAG = "TELEMETRY_ORCH";


void TelemetryOrchestrator::setRecordingMode(bool isRecording) {
    // Log current state and requested action
    ESP_LOGI(TAG, "Current state: %d, Requested recording: %s", _currentState, isRecording ? "START" : "STOP");
    if (isRecording && _currentState == SystemState::IDLE) {
        _currentState = SystemState::RECORDING;
        _logger->openLog();
        _indicator->indicateState(_currentState);
    }
    else if (!isRecording && _currentState == SystemState::RECORDING) {
        _currentState = SystemState::IDLE;
        _logger->closeLog();
        _indicator->indicateState(_currentState);
    }
}


TelemetryDTO TelemetryOrchestrator::quantize(const TelemetryDTO& rawPacket) {
    return rawPacket;
}


void TelemetryOrchestrator::processSensorData(const TelemetryDTO& rawPacket) {
    if (_currentState != SystemState::RECORDING) {
        return;
    }

    _logger->logRawPacket(rawPacket);

    TelemetryDTO processedPacket = quantize(rawPacket);

    _sender->sendPacket(processedPacket);
}
