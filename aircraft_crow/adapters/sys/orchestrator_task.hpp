#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "telemetry_dto.hpp"
#include "telemetry_orchestrator.hpp" 

class OrchestratorTask {
public:
    explicit OrchestratorTask(TelemetryOrchestrator* coreOrchestrator);
    ~OrchestratorTask();

    // Spawns the FreeRTOS task pinned to Core 1 (APP_CPU)
    bool start();

    // Thread-safe API for Core 0 hardware adapters (sensors/PWM) to push data
    bool pushEvent(const TelemetryDTO& event);

private:
    TelemetryOrchestrator* _coreOrchestrator;
    QueueHandle_t _eventQueue;
    TaskHandle_t _taskHandle;

    // FreeRTOS requires a static C-function for the task entry point
    static void taskEntry(void* pvParameters);
    
    // The actual instance loop where execution is handed off to the Core
    void runLoop();
};