#pragma once
#include "system_state.hpp"

class ISystemIndicator {
public:
    virtual ~ISystemIndicator() = default;
    
    virtual void indicateState(SystemState newState) = 0;
};