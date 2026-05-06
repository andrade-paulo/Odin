#pragma once

#include "adapter_mpu6050.hpp"
#include "adapter_mavlink_radio.hpp"
#include "adapter_rc_receiver.hpp"
#include "telemetry_scheduler.hpp"

struct AcqTaskParams {
    infra::MPU6050_Adapter* imu;
    core::TelemetryScheduler<infra::MavlinkRadioAdapter>* scheduler;
    infra::RCReceiverAdapter* rc_switch;
};