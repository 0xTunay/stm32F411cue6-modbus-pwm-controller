#ifndef DELAY_H
#define DELAY_H

#include "stdint.h"
#include "error_handler.h"

ModbusError_t SysTick_Init(void);
ModbusError_t SysTick_Delay(uint32_t ms);

#endif /* DELAY_H */