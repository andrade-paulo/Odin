#include <SPI.h>
#include <LoRa.h>

// Defina os pinos do seu ESP32 RECEPTOR
const int csPin = 5;      // Chip Select (NSS)
const int resetPin = 14;  // Reset
const int irqPin = 2;     // DIO0 (Interrupção)

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("\n=== Estação Base - Sniffer MAVLink v2 ===");
  Serial.println("Inicializando Rádio LoRa (915 MHz)...");

  LoRa.setPins(csPin, resetPin, irqPin);

  // Inicializa em 915MHz
  if (!LoRa.begin(915E6)) {
    Serial.println("FALHA: Rádio LoRa não encontrado. Verifique as conexões SPI.");
    while (1); // Trava aqui se der erro
  }

  // Coloca o rádio em modo de escuta contínua
  LoRa.receive();
  Serial.println("Rádio OK. Aguardando pacotes de telemetria...");
}

void loop() {
  // Verifica se há pacotes no buffer de hardware do SX1276
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    uint8_t buffer[256];
    int i = 0;
    
    // Lê todos os bytes do FIFO do LoRa
    while (LoRa.available() && i < 256) {
      buffer[i++] = (uint8_t)LoRa.read();
    }

    Serial.print("--------------------------------------------------\n");
    Serial.print("Pacote Recebido: ");
    Serial.print(packetSize);
    Serial.print(" bytes | RSSI: ");
    Serial.print(LoRa.packetRssi()); // Força do sinal (mais próximo de 0 é melhor)
    Serial.print(" dBm | SNR: ");
    Serial.println(LoRa.packetSnr());

    // Validação MAVLink v2
    if (packetSize > 0 && buffer[0] == 0xFD) {
        Serial.println("[OK] Magic Byte MAVLink v2 (0xFD) detectado.");
        
        // O byte 1 no MAVLink v2 indica o tamanho do Payload (excluindo header e CRC)
        int payloadLen = buffer[1];
        Serial.print("Tamanho real do Payload interno: ");
        Serial.print(payloadLen);
        Serial.println(" bytes");
    } else {
        Serial.println("[AVISO] Pacote não parece ser MAVLink v2 ou está corrompido.");
    }

    // Hex Dump do pacote completo para comparação com o log do transmissor
    Serial.print("Hex Dump: ");
    for (int j = 0; j < i; j++) {
      if (buffer[j] < 0x10) Serial.print("0");
      Serial.print(buffer[j], HEX);
      Serial.print(" ");
    }
    Serial.println("\n");
  }
}