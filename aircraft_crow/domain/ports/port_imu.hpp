#pragma once

class IImuSensor {
public:
    virtual ~IImuSensor() = default;
    
    virtual void calibrate() = 0;
    
    virtual bool isHealthy() = 0;
};