#ifndef RS485_H
#define RS485_H

#include <stdint.h>
#include "error_handler.h"

ModbusError_t RS485_Init(void);
ModbusError_t RS485_Transmit_Enable(void);
ModbusError_t RS485_Receive_Enable(void);
ModbusError_t RS485_SendChar(char ch);
ModbusError_t RS485_SendString(const char *str);

#endif /* RS485_H */