#pragma once

class ISensor {
public:
    virtual ~ISensor() = default;
    
    virtual void calibrate() = 0;
    
    virtual bool isHealthy() = 0;
};