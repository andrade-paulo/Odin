#pragma once

class IGpsSensor {
public:
    virtual ~IGpsSensor() = default;
    virtual bool isHealthy() = 0;
};