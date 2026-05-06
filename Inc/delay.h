#ifndef DELAY_H
#define DELAY_H

#include "stdint.h"

void SysTick_Init(void);
void SysTick_Delay(uint32_t ms);

#endif // DELAY_H