#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include "error_handler.h"

ModbusError_t PWM_Init(void);
ModbusError_t PWM_SetDutyPercent(uint8_t percent);

#endif /* PWM_H */
