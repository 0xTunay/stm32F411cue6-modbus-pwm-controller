#ifndef USART_H
#define USART_H

#include <stdint.h>

void USART2_Init(void);
void USART2_SendChar(char ch);
void USART2_SendString(const char* str);
char USART2_GetChar(void);
int USART2_GetCharTimeout(char *ch, uint32_t timeout_ms);
int USART2_ReadLineTimeout(char *buf, uint32_t bufsize, uint32_t timeout_ms);

#endif // USART_H