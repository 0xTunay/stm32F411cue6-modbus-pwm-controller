#ifndef USART_H
#define USART_H

void USART2_Init(void);
void USART2_SendChar(char ch);
void USART2_SendString(const char* str);
char USART2_GetChar(void);

#endif // USART_H