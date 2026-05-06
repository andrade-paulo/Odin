# Aircraft Crow — UAV Telemetry Node

Brief telemetry node for experimental UAV flight test using ESP32, IMU, LoRa and SD logging.

**Technologies**
- ESP-IDF (FreeRTOS + CMake)
- C++17
- Peripherals: I2C (MPU6050), SPI (SD card, SX1276 LoRa), GPIO PWM (RC input)

**Implementation details**
- Hexagonal design: adapters (infrastructure) implement the core ports.
- IMU adapter implements a CRTP `IMUPort` interface; main wiring calls the public `initialize()` / `is_healthy()`.
- RTOS tasks:
  - Acquisition task: reads IMU and schedules telemetry frames.
  - SD logger: batches CSV lines in RAM and flushes on timeout / periodic reopen to ensure FAT commits.
  - LoRa TX task: handles radio transmissions via `SX1276_Adapter`.
  - PWM logger: samples RC PWM and logs to SD/serial for debug.
- Recording is gated by RC PWM switch: PWM > 1500µs enables acquisition/enqueueing.

**How to use**

- Hardware setup (summary):
  - I2C (MPU6050): SDA=GPIO21, SCL=GPIO26
  - Shared SPI pins: MOSI=GPIO23, MISO=GPIO19, CLK=GPIO18
  - LoRa CS/RST/DIO0: CS=GPIO5, RST=GPIO14, DIO0=GPIO32
  - SD CS: GPIO27
  - RC PWM input: GPIO13
  - Buzzer: GPIO4
  (See `main/board_pinout_config.hpp` for exact constants.)

- Build and flash:
  - Install ESP-IDF and toolchain following Espressif docs.
  - From project root:
    ```bash
    idf.py build
    idf.py -p /dev/ttyUSB0 flash monitor
    ```

- Runtime usage:
  - On boot the wiring layer initializes I2C and SPI buses, mounts the SD card and brings up LoRa.
  - To start recording telemetry to SD, assert the RC switch (PWM > 1500µs). Acquisition will enqueue frames to the SD logger.
  - When PWM drops below 1500µs the acquisition task stops; the SD logger flushes buffered data on a 1s queue timeout and reopens the file periodically (≈10s) to force FAT metadata commit. Wait a few seconds after stopping recording before removing the card.
