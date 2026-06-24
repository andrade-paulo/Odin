#include "sx1276_driver.hpp"
#include "esp_log.h"
#include "esp_timer.h"
#include "rom/ets_sys.h"

static const char* TAG = "SX1276";

#define REG_FIFO                 0x00
#define REG_OP_MODE              0x01
#define REG_FRF_MSB              0x06
#define REG_PA_CONFIG            0x09
#define REG_FIFO_ADDR_PTR        0x0D
#define REG_FIFO_TX_BASE_ADDR    0x0E
#define REG_IRQ_FLAGS            0x12
#define REG_MODEM_CONFIG_1       0x1D
#define REG_MODEM_CONFIG_2       0x1E
#define REG_PAYLOAD_LENGTH       0x22
#define REG_DIO_MAPPING_1        0x40

// Modos de Operação (LoRa)
#define MODE_LONG_RANGE_MODE     0x80
#define MODE_SLEEP               0x00
#define MODE_STDBY               0x01
#define MODE_TX                  0x03

// IRQ Masks
#define IRQ_TX_DONE_MASK         0x08


Sx1276Driver::Sx1276Driver(spi_host_device_t spi_host, gpio_num_t cs, gpio_num_t rst, gpio_num_t dio0)
    : _spi_host(spi_host), 
      _spi(nullptr), 
      _cs_pin(cs), 
      _rst_pin(rst), 
      _dio0_pin(dio0) 
{
    _txDoneSem = xSemaphoreCreateBinary();
}

Sx1276Driver::~Sx1276Driver() {
    if (_spi) spi_bus_remove_device(_spi);
    if (_txDoneSem) vSemaphoreDelete(_txDoneSem);
}

void IRAM_ATTR Sx1276Driver::dio0InterruptHandler(void* arg) {
    Sx1276Driver* driver = static_cast<Sx1276Driver*>(arg);
    BaseType_t highTaskAwoken = pdFALSE;
    xSemaphoreGiveFromISR(driver->_txDoneSem, &highTaskAwoken);
    if (highTaskAwoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

bool Sx1276Driver::begin() {
    // Adicionar dispositivo ao barramento SPI (Máx 10MHz para SX1276)
    spi_device_interface_config_t devcfg = {};
    devcfg.clock_speed_hz = 8000000; 
    devcfg.mode = 0;                  // CPOL=0, CPHA=0
    devcfg.spics_io_num = _cs_pin;
    devcfg.queue_size = 3;

    if (spi_bus_add_device(_spi_host, &devcfg, &_spi) != ESP_OK) {
        ESP_LOGE(TAG, "Falha ao anexar SX1276 ao barramento SPI.");
        return false;
    }

    // Reset por Hardware
    gpio_set_direction(_rst_pin, GPIO_MODE_OUTPUT);
    gpio_set_level(_rst_pin, 0);
    ets_delay_us(1000); // 1ms
    gpio_set_level(_rst_pin, 1);
    vTaskDelay(pdMS_TO_TICKS(10)); // Aguarda 10ms pós reset

    // Checar a versão do chip (Para validar se a SPI está viva)
    uint8_t version = readRegister(0x42); // REG_VERSION
    if (version != 0x12) {
        ESP_LOGE(TAG, "SX1276 não encontrado. Versão lida: 0x%02X", version);
        return false;
    }

    // Configurar interrupção no DIO0 (Subida)
    gpio_config_t io_conf = {};
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.pin_bit_mask = (1ULL << _dio0_pin);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
    gpio_config(&io_conf);
    
    // Instala serviço de ISR global se ainda não existir na aplicação e anexa handler
    gpio_install_isr_service(0); 
    gpio_isr_handler_add(_dio0_pin, dio0InterruptHandler, (void*)this);

    // Configuração Base LoRa
    writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_SLEEP); // Força LoRa
    vTaskDelay(pdMS_TO_TICKS(10));
    writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY); // Mantém acordado em STDBY

    // Configurar Frequência para 915 MHz: (915E6 / (32E6 / 2^19)) = 14991360 = 0xE4C000
    writeRegister(REG_FRF_MSB, 0xE4);
    writeRegister(REG_FRF_MSB + 1, 0xC0);
    writeRegister(REG_FRF_MSB + 2, 0x00);

    // PA Config (TX Power Máximo para PA_BOOST)
    writeRegister(REG_PA_CONFIG, 0x8F); // PA_BOOST ativado, potência máxima

    ESP_LOGI(TAG, "SX1276 Inicializado a 915MHz.");
    return true;
}

bool Sx1276Driver::transmit(const uint8_t* buffer, size_t length) {
    if (length == 0 || length > 255) return false;

    // Entrar em Standby (Obrigatório antes de alterar FIFO)
    writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);

    // Mapear o DIO0 explicitamente para TxDone (Mapeamento 00 em DIO0)
    writeRegister(REG_DIO_MAPPING_1, 0x40);

    // Resetar e apontar o FIFO interno para a base de TX
    writeRegister(REG_FIFO_ADDR_PTR, 0x00);
    writeRegister(REG_FIFO_TX_BASE_ADDR, 0x00);
    writeRegister(REG_PAYLOAD_LENGTH, length);

    // Jogar a mensagem serializada via SPI Burst no FIFO
    writeBuffer(REG_FIFO, buffer, length);

    // Garantir que o semáforo está limpo antes de enviar
    xSemaphoreTake(_txDoneSem, 0); 

    // Engatilhar Transmissão
    writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_TX);

    // Esperar a Interrupção de hardware avisar que acabou (Timeout de segurança 1s)
    if (xSemaphoreTake(_txDoneSem, pdMS_TO_TICKS(1000)) == pdTRUE) {
        writeRegister(REG_IRQ_FLAGS, IRQ_TX_DONE_MASK); // Limpa a flag (Write 1 to clear)
        return true;
    } else {
        ESP_LOGE(TAG, "Timeout de Transmissão (Hardware travou ou desconectou)");
        // Força retorno ao standby em caso de falha
        writeRegister(REG_OP_MODE, MODE_LONG_RANGE_MODE | MODE_STDBY);
        return false;
    }
}

void Sx1276Driver::writeRegister(uint8_t reg, uint8_t value) {
    writeBuffer(reg, &value, 1);
}

uint8_t Sx1276Driver::readRegister(uint8_t reg) {
    uint8_t tx_data[2] = { (uint8_t)(reg & 0x7F), 0x00 }; // MSB = 0 para Read
    uint8_t rx_data[2] = { 0 };

    spi_transaction_t t = {};
    t.length = 16;
    t.tx_buffer = tx_data;
    t.rx_buffer = rx_data;

    spi_device_transmit(_spi, &t);
    return rx_data[1];
}

void Sx1276Driver::writeBuffer(uint8_t reg, const uint8_t* buffer, size_t length) {
    // Aloca um buffer temporário para SPI (Registro + Dados)
    uint8_t tx_data[257];
    tx_data[0] = reg | 0x80;
    memcpy(&tx_data[1], buffer, length);

    spi_transaction_t t = {};
    t.length = (length + 1) * 8; // length in bits
    t.tx_buffer = tx_data;

    spi_device_transmit(_spi, &t);
}