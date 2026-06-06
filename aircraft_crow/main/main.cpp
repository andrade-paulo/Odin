#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "telemetry_orchestrator.hpp"
#include "orchestrator_task.hpp"
#include "uart_hil_task.hpp"

#include "port_telemetry_sender.hpp"
#include "port_telemetry_logger.hpp"
#include "port_system_indicator.hpp"

static const char* TAG = "APP_MAIN";


class MockTelemetrySender : public ITelemetrySender {
public:
    void sendPacket(const TelemetryDTO& packet) override {
        // Simulando a transmissão LoRa/MAVLink
        ESP_LOGI("LORA_MOCK", "Enviando pacote via Rádio. Tipo: %d", static_cast<int>(packet.type));
    }
};


class MockTelemetryLogger : public ITelemetryLogger {
public:
    void openLog() override { 
        ESP_LOGW("SD_MOCK", "=== Arquivo de Log no Cartão SD ABERTO ==="); 
    }
    
    void logRawPacket(const TelemetryDTO& packet) override {
        // Apenas para visualização em bancada. Em voo, remova logs excessivos.
        if(packet.type == MessageType::IMU) {
            ESP_LOGI("SD_MOCK", "Gravado no SD -> IMU | Accel_Z: %.2f", packet.payload.imu.linear_acceleration_z);
        } else if (packet.type == MessageType::BARO) {
            ESP_LOGI("SD_MOCK", "Gravado no SD -> BARO | Alt: %.2f m", packet.payload.barometer.altitude);
        }
    }
    
    void closeLog() override { 
        ESP_LOGW("SD_MOCK", "=== Arquivo de Log no Cartão SD FECHADO ==="); 
    }
};


class MockSystemIndicator : public ISystemIndicator {
public:
    void indicateState(SystemState newState) override {
        ESP_LOGW("LED_MOCK", "Estado alterado! Novo estado: %d", static_cast<int>(newState));
    }
};


extern "C" void app_main() {
    ESP_LOGI(TAG, "===============================================");
    ESP_LOGI(TAG, " Inicializando Sistema de Telemetria (HIL Mode)");
    ESP_LOGI(TAG, "===============================================");

    // 1. Instanciar os Adaptadores (Injeção de Dependências)
    MockTelemetrySender* sender = new MockTelemetrySender();
    MockTelemetryLogger* logger = new MockTelemetryLogger();
    MockSystemIndicator* indicator = new MockSystemIndicator();

    // 2. Instanciar o Core (A Lógica de Domínio pura em C++)
    TelemetryOrchestrator* coreOrchestrator = new TelemetryOrchestrator(sender, logger, indicator);

    // 3. Instanciar e iniciar a Task do Orchestrator (Fixada no Core 1 - APP_CPU)
    OrchestratorTask* orchestratorTask = new OrchestratorTask(coreOrchestrator);
    if (orchestratorTask->start()) {
        ESP_LOGI(TAG, "[OK] OrchestratorTask rodando no Core 1.");
    } else {
        ESP_LOGE(TAG, "[FALHA] Não foi possível iniciar a OrchestratorTask.");
    }

    // 4. Instanciar e iniciar a Task do Simulador HIL (Fixada no Core 0 - PRO_CPU)
    // Passamos a orchestratorTask para que ela possa empurrar os dados para a fila.
    UartHilTask* hilTask = new UartHilTask(orchestratorTask);
    if (hilTask->start()) {
        ESP_LOGI(TAG, "[OK] UartHilTask (Emulador HIL) rodando no Core 0 na UART2.");
    } else {
        ESP_LOGE(TAG, "[FALHA] Não foi possível iniciar a UartHilTask.");
    }

    // 5. A thread principal (app_main) agora é desnecessária. 
    // Podemos deletá-la ou mantê-la como um watchdog/monitor de vida simples.
    while (true) {
        vTaskDelay(pdMS_TO_TICKS(10000)); // Aguarda 10 segundos
        ESP_LOGD(TAG, "Sistema rodando perfeitamente... Monitor de tasks vivo.");
    }
}