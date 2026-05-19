// Task parameters, static stacks and priorities
#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Priorities
static constexpr UBaseType_t LORA_TASK_PRIORITY = 3;
static constexpr UBaseType_t UDP_TASK_PRIORITY = 4;

// Stack sizes (in words)
static constexpr uint32_t LORA_TASK_STACK_WORDS = 4096/sizeof(StackType_t);
static constexpr uint32_t UDP_TASK_STACK_WORDS = 4096/sizeof(StackType_t);

// Static stacks and TCBs
extern StackType_t lora_task_stack[];
extern StaticTask_t lora_task_tcb;

extern StackType_t udp_task_stack[];
extern StaticTask_t udp_task_tcb;

