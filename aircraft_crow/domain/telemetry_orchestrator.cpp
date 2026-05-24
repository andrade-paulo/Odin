#include "telemetry_orchestrator.hpp"


void TelemetryOrchestrator::finishCalibration() {
    if (_currentState == SystemState::CALIBRATING) {
        _currentState = SystemState::IDLE;
        _indicator->indicateState(_currentState);
    }
}


void TelemetryOrchestrator::setRecordingMode(bool isRecording) {
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


void TelemetryOrchestrator::processSensorData(const TelemetryDTO& rawPacket) {
    if (_currentState != SystemState::RECORDING) {
        return;
    }

    _logger->logRawPacket(rawPacket);

    TelemetryDTO processedPacket = applyFiltersAndQuantize(rawPacket);

    _sender->sendPacket(processedPacket);
}
