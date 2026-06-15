#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "telemetry_orchestrator.hpp"
#include "orchestrator_task.hpp"
#include "producer_uart_hil_task.hpp"
#include "consumer_uart_hil_task.hpp" // Novo Consumidor

#include "port_telemetry_logger.hpp"
#include "port_system_indicator.hpp"

static const char* TAG = "APP_MAIN";

class MockTelemetryLogger : public ITelemetryLogger {
public:
    void openLog() override { ESP_LOGW("SD_MOCK", "=== SD ABERTO ==="); }
    void logRawPacket(const TelemetryDTO& packet) override {}
    void closeLog() override { ESP_LOGW("SD_MOCK", "=== SD FECHADO ==="); }
};

class MockSystemIndicator : public ISystemIndicator {
public:
    void indicateState(SystemState newState) override {
        ESP_LOGW("LED_MOCK", "Estado alterado: %d", static_cast<int>(newState));
    }
};

extern "C" void app_main() {
    ESP_LOGI(TAG, "===============================================");
    ESP_LOGI(TAG, " Inicializando Sistema de Telemetria (HIL Mode)");
    ESP_LOGI(TAG, "===============================================");

    MockTelemetryLogger* logger = new MockTelemetryLogger();
    MockSystemIndicator* indicator = new MockSystemIndicator();
    
    ConsumerUartHilTask* consumerTask = new ConsumerUartHilTask();
    if (consumerTask->start()) {
        ESP_LOGI(TAG, "[OK] ConsumerUartHilTask rodando no Core 0.");
    } else {
        ESP_LOGI(TAG, "[ERROR] Consumer HIL task falhou");
    }

    TelemetryOrchestrator* coreOrchestrator = new TelemetryOrchestrator(consumerTask, logger, indicator);

    OrchestratorTask* orchestratorTask = new OrchestratorTask(coreOrchestrator);
    if (orchestratorTask->start()) {
        ESP_LOGI(TAG, "[OK] OrchestratorTask rodando no Core 1.");
    } else {
        ESP_LOGI(TAG, "[ERROR] Orchestrator task falhou");
    }

    ProducerUartHilTask* hilTask = new ProducerUartHilTask(orchestratorTask);
    if (hilTask->start()) {
        ESP_LOGI(TAG, "[OK] ProducerUartHilTask rodando no Core 0.");
    }

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(10000));
        ESP_LOGD(TAG, "Sistema rodando.");
    }
}