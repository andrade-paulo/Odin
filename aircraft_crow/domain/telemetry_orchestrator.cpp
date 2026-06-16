#include "telemetry_orchestrator.hpp"

static const char* TAG = "TELEMETRY_ORCH";


void TelemetryOrchestrator::finishCalibration() {
    if (_currentState == SystemState::CALIBRATING) {
        _currentState = SystemState::IDLE;
        _indicator->indicateState(_currentState);
    }
}


void TelemetryOrchestrator::setRecordingMode(bool isRecording) {
    // Log current state and requested action
    ESP_LOGI(TAG, "Current state: %d, Requested recording: %s", _currentState, isRecording ? "START" : "STOP");
    if (isRecording && _currentState == SystemState::IDLE) {
        _currentState = SystemState::RECORDING;
        _logger->openLog();
        _indicator->indicateState(_currentState);
        ESP_LOGI(TAG, "System recording");
    }
    else if (!isRecording && _currentState == SystemState::RECORDING) {
        _currentState = SystemState::IDLE;
        _logger->closeLog();
        _indicator->indicateState(_currentState);
        ESP_LOGI(TAG, "System not recording");
    }

    ESP_LOGI(TAG, "Nenhum dos dois :(");
}


TelemetryDTO TelemetryOrchestrator::applyFiltersAndQuantize(const TelemetryDTO& rawPacket) {
    return rawPacket;
}


void TelemetryOrchestrator::processSensorData(const TelemetryDTO& rawPacket) {
    if (_currentState != SystemState::RECORDING) {
        return;
    }

    _logger->logRawPacket(rawPacket);

    TelemetryDTO processedPacket = applyFiltersAndQuantize(rawPacket);

    _sender->sendPacket(processedPacket);
}
