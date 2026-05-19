
#pragma once

// SX1276 LoRa SPI pinout for ESP32.
// Update the radio settings below to match the sender.
#define SCK_PIN  18
#define MISO_PIN 19
#define MOSI_PIN 23
#define CS_PIN   5
#define RST_PIN  14
#define DIO0_PIN 26

// Default LoRa modem settings.
// Frequency must match the sender exactly.
#define LORA_RF_FREQUENCY_HZ 915000000UL
#define LORA_SYNC_WORD 0x34
#define LORA_BW_125KHZ 125000UL
#define LORA_SPREADING_FACTOR 7
#define LORA_CODING_RATE_4_5 1
