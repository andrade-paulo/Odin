#pragma once

class IBarometerSensor {
public:
    virtual ~IBarometerSensor() = default;
    virtual bool isHealthy() = 0;
};
