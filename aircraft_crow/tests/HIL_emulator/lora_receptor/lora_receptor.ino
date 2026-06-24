#include <SPI.h>
#include <LoRa.h>

// Definição dos pinos do ESP32 RECEPTOR
const int csPin = 5;
const int resetPin = 14;
const int irqPin = 34;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Imprime o cabeçalho do CSV uma única vez na inicialização
  Serial.println("timestamp,RSSI,SNR,Is_Valid_Mavlink,Package_Size,Package_Hex");

  LoRa.setPins(csPin, resetPin, irqPin);

  // Inicializa o rádio em 915MHz
  if (!LoRa.begin(915E6)) {
    // Falha crítica de hardware - Trava a execução
    while (1);
  }

  // Coloca o rádio em modo de escuta contínua
  LoRa.receive();
}

void loop() {
  // Verifica se há pacotes no buffer do hardware
  int packetSize = LoRa.parsePacket();

  if (packetSize) {
    uint8_t buffer[256];
    int i = 0;

    // Lê todos os bytes do FIFO do LoRa
    while (LoRa.available() && i < 256) {
      buffer[i++] = (uint8_t)LoRa.read();
    }

    // Coleta de Metadados
    unsigned long timestamp = millis();
    int rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();

    // Validação MAVLink v2 (Verifica se o pacote é maior que 0 e começa com 0xFD)
    int isValidMavlink = (packetSize > 0 && buffer[0] == 0xFD) ? 1 : 0;

    // --- SAÍDA FORMATADA EM CSV ---
    Serial.print(timestamp);
    Serial.print(",");
    Serial.print(rssi);
    Serial.print(",");
    Serial.print(snr);
    Serial.print(",");
    Serial.print(isValidMavlink);
    Serial.print(",");
    Serial.print(packetSize);
    Serial.print(",");

    // Hex Dump contínuo (sem espaços) para formar uma única string na coluna
    for (int j = 0; j < i; j++) {
      if (buffer[j] < 0x10) Serial.print("0"); // Padding para manter 2 caracteres por byte
      Serial.print(buffer[j], HEX);
    }

    // Quebra de linha para finalizar o registro (linha do CSV)
    Serial.println();
  }
}
