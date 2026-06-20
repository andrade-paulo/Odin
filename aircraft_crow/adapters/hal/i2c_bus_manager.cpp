#include "i2c_bus_manager.hpp"
#include "esp_log.h"

static const char* TAG = "I2C_BUS_MANAGER";

I2cBusManager::I2cBusManager(i2c_port_t port, int sdaPin, int sclPin, uint32_t clockSpeedHz)
    : _port(port), 
      _sdaPin(sdaPin), 
      _sclPin(sclPin), 
      _clockSpeedHz(clockSpeedHz),
      _mutex(nullptr),
      _timeoutTicks(pdMS_TO_TICKS(10)) 
{
}

I2cBusManager::~I2cBusManager() {
    if (_mutex != nullptr) {
        vSemaphoreDelete(_mutex);
    }
    i2c_driver_delete(_port);
    ESP_LOGI(TAG, "Driver I2C desinstalado e Mutex destruido.");
}

bool I2cBusManager::init() {
    _mutex = xSemaphoreCreateMutex();
    if (_mutex == nullptr) {
        ESP_LOGE(TAG, "Falha fatal: Nao foi possivel criar o Mutex do I2C.");
        return false;
    }

    i2c_config_t conf = {};
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = _sdaPin;
    conf.scl_io_num = _sclPin;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = _clockSpeedHz;
    conf.clk_flags = 0;

    esp_err_t err = i2c_param_config(_port, &conf);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Erro ao configurar os parametros I2C: %s", esp_err_to_name(err));
        return false;
    }

    err = i2c_driver_install(_port, conf.mode, 0, 0, 0);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Erro ao instalar o driver nativo I2C: %s", esp_err_to_name(err));
        return false;
    }

    ESP_LOGI(TAG, "Barramento I2C inicializado (Porta %d, SDA %d, SCL %d, %lu Hz)", 
             _port, _sdaPin, _sclPin, _clockSpeedHz);
    return true;
}

bool I2cBusManager::writeRegister(uint8_t deviceAddress, uint8_t regAddress, uint8_t data) {
    if (xSemaphoreTake(_mutex, _timeoutTicks) != pdTRUE) {
        ESP_LOGW(TAG, "Timeout ao tentar obter Mutex para escrita no disp 0x%02X", deviceAddress);
        return false;
    }

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceAddress << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, regAddress, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);

    esp_err_t err = i2c_master_cmd_begin(_port, cmd, _timeoutTicks);
    i2c_cmd_link_delete(cmd); // Obrigatório para evitar memory leak
    
    xSemaphoreGive(_mutex);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao escrever no registrador 0x%02X do disp 0x%02X", regAddress, deviceAddress);
        return false;
    }
    return true;
}

bool I2cBusManager::readRegister(uint8_t deviceAddress, uint8_t regAddress, uint8_t& data) {
    return readRegisters(deviceAddress, regAddress, &data, 1);
}

bool I2cBusManager::readRegisters(uint8_t deviceAddress, uint8_t regAddress, uint8_t* dataBuffer, size_t length) {
    if (length == 0 || dataBuffer == nullptr) {
        return false;
    }

    if (xSemaphoreTake(_mutex, _timeoutTicks) != pdTRUE) {
        ESP_LOGW(TAG, "Timeout ao tentar obter Mutex para leitura no disp 0x%02X", deviceAddress);
        return false;
    }

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    
    // Escreve o endereço do registrador que queremos ler
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceAddress << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, regAddress, true);
    
    // Repeated Start para trocar a direção do barramento para leitura
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceAddress << 1) | I2C_MASTER_READ, true);
    
    if (length > 1) {
        // Lê os N-1 bytes enviados com ACK (Acknowledge)
        i2c_master_read(cmd, dataBuffer, length - 1, I2C_MASTER_ACK);
    }
    // O último byte obrigatoriamente deve terminar com NACK para avisar o sensor que a leitura acabou
    i2c_master_read_byte(cmd, dataBuffer + length - 1, I2C_MASTER_NACK);
    
    i2c_master_stop(cmd);

    esp_err_t err = i2c_master_cmd_begin(_port, cmd, _timeoutTicks);
    i2c_cmd_link_delete(cmd); // Obrigatório para evitar memory leak

    xSemaphoreGive(_mutex);

    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Falha de leitura (Cod: %s) no registrador 0x%02X do disp 0x%02X", 
                 esp_err_to_name(err), regAddress, deviceAddress);
        return false;
    }
    return true;
}

bool I2cBusManager::writeCommand(uint8_t deviceAddress, uint8_t command) {
    if (xSemaphoreTake(_mutex, _timeoutTicks) != pdTRUE) return false;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (deviceAddress << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, command, true);
    i2c_master_stop(cmd);
    esp_err_t err = i2c_master_cmd_begin(_port, cmd, _timeoutTicks);
    i2c_cmd_link_delete(cmd);
    xSemaphoreGive(_mutex);
    return (err == ESP_OK);
}