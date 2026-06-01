#include "orchestrator_task.hpp"


constexpr uint32_t QUEUE_LENGTH = 50;      // BTODO: refine this based on expected sensor rates and processing time
constexpr uint32_t TASK_STACK_SIZE = 4096; // Adjust based on filtering math requirements
constexpr uint8_t TASK_PRIORITY = 5;       // High priority for the main logic loop
constexpr BaseType_t CORE_1 = 1;           // Pin to APP_CPU

OrchestratorTask::OrchestratorTask(TelemetryOrchestrator* coreOrchestrator) 
    : _coreOrchestrator(coreOrchestrator), _taskHandle(nullptr) {
    
    _eventQueue = xQueueCreate(QUEUE_LENGTH, sizeof(TelemetryDTO));
}

OrchestratorTask::~OrchestratorTask() {
    if (_eventQueue != nullptr) {
        vQueueDelete(_eventQueue);
    }
    if (_taskHandle != nullptr) {
        vTaskDelete(_taskHandle);
    }
}


bool OrchestratorTask::start() {
    if (_eventQueue == nullptr) return false;

    BaseType_t result = xTaskCreatePinnedToCore(
        OrchestratorTask::taskEntry,
        "OrchestratorTask",
        TASK_STACK_SIZE,
        this,             // Pass the class instance pointer to the static task
        TASK_PRIORITY,
        &_taskHandle,
        CORE_1            
    );

    return result == pdPASS;
}


bool OrchestratorTask::pushEvent(const TelemetryDTO& event) {
    if (_eventQueue == nullptr) return false;

    // Using a 0-tick timeout ensures fast sensor tasks on Core 0 never stall.
    // If the queue is full, the oldest data drops.
    return xQueueSend(_eventQueue, &event, 0) == pdTRUE;
}


void OrchestratorTask::taskEntry(void* pvParameters) {
    // Cast the void pointer back to our class instance
    OrchestratorTask* instance = static_cast<OrchestratorTask*>(pvParameters);
    
    instance->runLoop();
}


void OrchestratorTask::runLoop() {
    TelemetryDTO incomingEvent;

    while (true) {
        // Block indefinitely until an adapter pushes an event into the queue
        if (xQueueReceive(_eventQueue, &incomingEvent, portMAX_DELAY) == pdTRUE) {
            
            // Route the event into the domain
            if (incomingEvent.type == MessageType::COMMAND) {
                // Handle the PWM state trigger
                bool start = (incomingEvent.payload.command == CommandType::START_RECORDING);
                _coreOrchestrator->setRecordingMode(start);
            } 
            else {
                // Feed sensor data to the FSM
                _coreOrchestrator->processSensorData(incomingEvent);
            }
        }
    }
}