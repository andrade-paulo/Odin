#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// Includes da sua Arquitetura
#include "telemetry_orchestrator.hpp"
#include "orchestrator_task.hpp"
#include "i2c_bus_manager.hpp"
#include "imu_task.hpp"
#include "barometer_task.hpp"
#include "gps_task.hpp"
// Include do seu sender HIL existente
#include "consumer_uart_hil_task.hpp" 
#include "driver/i2c.h"


void scan_i2c_bus() {
    ESP_LOGI("SCANNER", "Iniciando varredura I2C...");
    int devices_found = 0;
    for (uint8_t i = 1; i < 127; i++) {
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        i2c_master_start(cmd);
        i2c_master_write_byte(cmd, (i << 1) | I2C_MASTER_WRITE, true);
        i2c_master_stop(cmd);
        esp_err_t err = i2c_master_cmd_begin(I2C_NUM_0, cmd, pdMS_TO_TICKS(20));
        i2c_cmd_link_delete(cmd);
        
        if (err == ESP_OK) {
            ESP_LOGW("SCANNER", ">>> SENSOR ENCONTRADO NO ENDERECO: 0x%02X <<<", i);
            devices_found++;
        }
    }
    ESP_LOGI("SCANNER", "Varredura concluida. Total: %d dispositivos", devices_found);
}


extern "C" void app_main(void) {
    ESP_LOGI("MAIN", "Iniciando Sistema de Telemetria Odin...");

    auto* uartSender = new ConsumerUartHilTask(); 

    auto* core = new TelemetryOrchestrator(uartSender, nullptr, nullptr, nullptr);
    auto* orchestratorTask = new OrchestratorTask(core);

    auto* i2cBus = new I2cBusManager(I2C_NUM_0, 21, 22, 400000); // 400kHz
    if (!i2cBus->init()) {
        ESP_LOGE("MAIN", "Falha catastrofica no I2C. Abortando sensores.");
        // Estado de erro
    }

    //scan_i2c_bus();
    //while(1) { vTaskDelay(1000); }

    auto* imu = new ImuTask(orchestratorTask, i2cBus);
    auto* baro = new BarometerTask(orchestratorTask, i2cBus);
    auto* gps = new GpsTask(orchestratorTask, UART_NUM_2, 17, 16);

    uartSender->start();
    orchestratorTask->start();
    
    vTaskDelay(pdMS_TO_TICKS(100)); 

    imu->start();
    baro->start();
    gps->start();

    ESP_LOGI("MAIN", "Kernel operacional. Sensores ativos.");
    
    // A app_main pode encerrar ou entrar em loop ocioso
    vTaskDelete(NULL); 
}