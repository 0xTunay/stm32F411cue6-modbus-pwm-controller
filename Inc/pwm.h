#ifndef PWM_H
#define PWM_H

#include <stdint.h>

void PWM_Init(void);
void PWM_SetDutyPercent(uint8_t percent);

#endif /* PWM_H */
