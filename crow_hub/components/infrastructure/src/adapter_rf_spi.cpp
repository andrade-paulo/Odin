#include "adapter_rf_spi.hpp"
#include "../../../main/board_pinout_config.hpp"
#include "esp_log.h"
#include <driver/gpio.h>
#include <driver/spi_master.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string.h>

static const char* TAG = "AdapterRfSpi";

namespace {
static constexpr spi_host_device_t kSpiHost = SPI3_HOST;
static constexpr int kSpiClockHz = 8 * 1000 * 1000;

static constexpr uint8_t kRegFifo = 0x00;
static constexpr uint8_t kRegOpMode = 0x01;
static constexpr uint8_t kRegFrMsb = 0x06;
static constexpr uint8_t kRegFrMid = 0x07;
static constexpr uint8_t kRegFrLsb = 0x08;
static constexpr uint8_t kRegPaConfig = 0x09;
static constexpr uint8_t kRegLna = 0x0C;
static constexpr uint8_t kRegFifoAddrPtr = 0x0D;
static constexpr uint8_t kRegFifoTxBaseAddr = 0x0E;
static constexpr uint8_t kRegFifoRxBaseAddr = 0x0F;
static constexpr uint8_t kRegFifoRxCurrentAddr = 0x10;
static constexpr uint8_t kRegIrqFlags = 0x12;
static constexpr uint8_t kRegRxNbBytes = 0x13;
static constexpr uint8_t kRegPktSnrValue = 0x19;
static constexpr uint8_t kRegPktRssiValue = 0x1A;
static constexpr uint8_t kRegModemConfig1 = 0x1D;
static constexpr uint8_t kRegModemConfig2 = 0x1E;
static constexpr uint8_t kRegPreambleMsb = 0x20;
static constexpr uint8_t kRegPreambleLsb = 0x21;
static constexpr uint8_t kRegModemConfig3 = 0x26;
static constexpr uint8_t kRegSyncWord = 0x39;
static constexpr uint8_t kRegDioMapping1 = 0x40;
static constexpr uint8_t kRegVersion = 0x42;

static constexpr uint8_t kIrqRxDone = 0x40;
static constexpr uint8_t kIrqPayloadCrcError = 0x20;

static constexpr uint8_t kOpModeLoRaSleep = 0x80;
static constexpr uint8_t kOpModeLoRaStandby = 0x81;
static constexpr uint8_t kOpModeLoRaRxContinuous = 0x85;

static constexpr uint32_t kRadioFreqHz = LORA_RF_FREQUENCY_HZ;
static constexpr uint8_t kSyncWord = LORA_SYNC_WORD;
static constexpr uint8_t kModemConfig1 = 0x72; // BW 125 kHz, CR 4/5, explicit header
static constexpr uint8_t kModemConfig2 = static_cast<uint8_t>((LORA_SPREADING_FACTOR << 4) | 0x04); // SF + CRC on
static constexpr uint8_t kModemConfig3 = 0x04; // AGC auto on
static constexpr uint8_t kPreambleMsb = 0x00;
static constexpr uint8_t kPreambleLsb = 0x08;
}

bool AdapterRfSpi::write_register(uint8_t reg, uint8_t value)
{
	uint8_t tx[2] = { static_cast<uint8_t>(reg | 0x80), value };
	spi_transaction_t transaction = {};
	transaction.length = sizeof(tx) * 8;
	transaction.tx_buffer = tx;
	return spi_device_transmit(spi_device_, &transaction) == ESP_OK;
}

bool AdapterRfSpi::read_register(uint8_t reg, uint8_t* value)
{
	if (value == nullptr) {
		return false;
	}
	uint8_t tx[2] = { static_cast<uint8_t>(reg & 0x7F), 0x00 };
	uint8_t rx[2] = { 0 };
	spi_transaction_t transaction = {};
	transaction.length = sizeof(tx) * 8;
	transaction.tx_buffer = tx;
	transaction.rx_buffer = rx;
	if (spi_device_transmit(spi_device_, &transaction) != ESP_OK) {
		return false;
	}
	*value = rx[1];
	return true;
}

bool AdapterRfSpi::burst_read(uint8_t reg, uint8_t* buffer, size_t len)
{
	if (buffer == nullptr || len == 0) {
		return false;
	}
	uint8_t tx[256 + 1] = {0};
	uint8_t rx[256 + 1] = {0};
	const size_t copy_len = len > 255 ? 255 : len;
	tx[0] = static_cast<uint8_t>(reg & 0x7F);
	spi_transaction_t transaction = {};
	transaction.length = (copy_len + 1) * 8;
	transaction.tx_buffer = tx;
	transaction.rx_buffer = rx;
	if (spi_device_transmit(spi_device_, &transaction) != ESP_OK) {
		return false;
	}
	memcpy(buffer, &rx[1], copy_len);
	return true;
}

bool AdapterRfSpi::reset_radio()
{
	gpio_set_level(static_cast<gpio_num_t>(RST_PIN), 0);
	vTaskDelay(pdMS_TO_TICKS(10));
	gpio_set_level(static_cast<gpio_num_t>(RST_PIN), 1);
	vTaskDelay(pdMS_TO_TICKS(10));
	return true;
}

bool AdapterRfSpi::configure_radio()
{
	if (!write_register(kRegOpMode, kOpModeLoRaSleep)) {
		return false;
	}
	vTaskDelay(pdMS_TO_TICKS(10));

	if (!write_register(kRegOpMode, kOpModeLoRaStandby)) {
		return false;
	}

	const uint64_t frf = (static_cast<uint64_t>(kRadioFreqHz) << 19) / 32000000ULL;
	if (!write_register(kRegFrMsb, static_cast<uint8_t>((frf >> 16) & 0xFF)) ||
		!write_register(kRegFrMid, static_cast<uint8_t>((frf >> 8) & 0xFF)) ||
		!write_register(kRegFrLsb, static_cast<uint8_t>(frf & 0xFF))) {
		return false;
	}

	if (!write_register(kRegPaConfig, 0x00) ||
		!write_register(kRegLna, 0x23) ||
		!write_register(kRegFifoTxBaseAddr, 0x00) ||
		!write_register(kRegFifoRxBaseAddr, 0x00) ||
		!write_register(kRegModemConfig1, kModemConfig1) ||
		!write_register(kRegModemConfig2, kModemConfig2) ||
		!write_register(kRegModemConfig3, kModemConfig3) ||
		!write_register(kRegPreambleMsb, kPreambleMsb) ||
		!write_register(kRegPreambleLsb, kPreambleLsb) ||
		!write_register(kRegSyncWord, kSyncWord) ||
		!write_register(kRegDioMapping1, 0x00) ||
		!write_register(kRegIrqFlags, 0xFF)) {
		return false;
	}

	if (!write_register(kRegOpMode, kOpModeLoRaRxContinuous)) {
		return false;
	}

	ESP_LOGI(TAG, "SX1276 configured for RX: freq=%lu Hz, SF=%u, sync=0x%02X", static_cast<unsigned long>(kRadioFreqHz), LORA_SPREADING_FACTOR, kSyncWord);
	return true;
}

bool AdapterRfSpi::init_impl()
{
	if (initialized_) {
		return true;
	}

	gpio_config_t reset_config = {};
	reset_config.pin_bit_mask = 1ULL << RST_PIN;
	reset_config.mode = GPIO_MODE_OUTPUT;
	reset_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
	reset_config.pull_up_en = GPIO_PULLUP_DISABLE;
	if (gpio_config(&reset_config) != ESP_OK) {
		ESP_LOGE(TAG, "Failed to configure RST pin");
		return false;
	}

	gpio_config_t dio0_config = {};
	dio0_config.pin_bit_mask = 1ULL << DIO0_PIN;
	dio0_config.mode = GPIO_MODE_INPUT;
	dio0_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
	dio0_config.pull_up_en = GPIO_PULLUP_DISABLE;
	if (gpio_config(&dio0_config) != ESP_OK) {
		ESP_LOGE(TAG, "Failed to configure DIO0 pin");
		return false;
	}

	spi_bus_config_t bus_config = {};
	bus_config.sclk_io_num = SCK_PIN;
	bus_config.mosi_io_num = MOSI_PIN;
	bus_config.miso_io_num = MISO_PIN;
	bus_config.quadwp_io_num = -1;
	bus_config.quadhd_io_num = -1;
	bus_config.max_transfer_sz = 256;

	esp_err_t err = spi_bus_initialize(kSpiHost, &bus_config, SPI_DMA_CH_AUTO);
	if (err != ESP_OK && err != ESP_ERR_INVALID_STATE) {
		ESP_LOGE(TAG, "spi_bus_initialize failed: %d", err);
		return false;
	}

	spi_device_interface_config_t device_config = {};
	device_config.clock_speed_hz = kSpiClockHz;
	device_config.mode = 0;
	device_config.spics_io_num = CS_PIN;
	device_config.queue_size = 1;

	err = spi_bus_add_device(kSpiHost, &device_config, &spi_device_);
	if (err != ESP_OK) {
		ESP_LOGE(TAG, "spi_bus_add_device failed: %d", err);
		return false;
	}

	reset_radio();

	uint8_t version = 0;
	if (!read_register(kRegVersion, &version)) {
		ESP_LOGE(TAG, "Failed to read SX1276 version register");
		return false;
	}
	if (version != 0x12) {
		ESP_LOGW(TAG, "Unexpected SX1276 version: 0x%02X (expected 0x12)", version);
	}

	if (!configure_radio()) {
		ESP_LOGE(TAG, "SX1276 configuration failed");
		return false;
	}

	initialized_ = true;
	ESP_LOGI(TAG, "SX1276 SPI adapter initialized");
	return true;
}

size_t AdapterRfSpi::read_bytes_impl(uint8_t* buffer, size_t max_len)
{
	if (!initialized_ || buffer == nullptr || max_len == 0) {
		return 0;
	}

	uint8_t irq_flags = 0;
	if (!read_register(kRegIrqFlags, &irq_flags)) {
		return 0;
	}

	if ((irq_flags & kIrqRxDone) == 0) {
		return 0;
	}

	if ((irq_flags & kIrqPayloadCrcError) != 0) {
		write_register(kRegIrqFlags, 0xFF);
		ESP_LOGW(TAG, "CRC error on received LoRa packet");
		return 0;
	}

	uint8_t current_addr = 0;
	uint8_t packet_len = 0;
	if (!read_register(kRegFifoRxCurrentAddr, &current_addr) ||
		!read_register(kRegRxNbBytes, &packet_len)) {
		return 0;
	}

	if (packet_len == 0) {
		write_register(kRegIrqFlags, 0xFF);
		return 0;
	}

	const size_t copy_len = packet_len > max_len ? max_len : packet_len;
	if (!write_register(kRegFifoAddrPtr, current_addr)) {
		return 0;
	}
	if (!burst_read(kRegFifo, buffer, copy_len)) {
		return 0;
	}

	write_register(kRegIrqFlags, 0xFF);
	return copy_len;
}
