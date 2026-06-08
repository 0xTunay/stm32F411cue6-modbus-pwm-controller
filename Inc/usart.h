#ifndef USART_H
#define USART_H

#include <stdint.h>
#include "error_handler.h"

ModbusError_t USART2_Init(void);
ModbusError_t USART2_SendChar(char ch);
ModbusError_t USART2_SendString(const char* str);
ModbusError_t USART2_GetChar(char *ch);
ModbusError_t USART2_GetCharTimeout(char *ch, uint32_t timeout_ms);
ModbusError_t USART2_ReadLineTimeout(char *buf, uint32_t bufsize, uint32_t timeout_ms);

#endif /* USART_H */
