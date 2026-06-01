#ifndef RS485_H
#define RS485_H

#include <stdint.h>

void RS485_Init(void);
void RS485_Transmit_Enable(void);
void RS485_Receive_Enable(void);
void RS485_SendChar(char ch);
void RS485_SendString(const char *str);


#endif /* RS485_H */