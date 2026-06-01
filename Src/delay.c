#include "delay.h"
#include "stm32f4xx.h"

extern void Modbus_TimerTick(void);

static volatile uint32_t delay_tick = 0;

void SysTick_Handler(void) {
    if (delay_tick > 0) {
        delay_tick--;
    }
    Modbus_TimerTick();
}

void SysTick_Init(void) {
    /* SystemCoreClock is set in clock_init() */
    SysTick->LOAD = (SystemCoreClock / 1000) - 1;   /* 1ms tick */
    SysTick->VAL  = 0;                              /* reset counter */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |    /* use CPU clock */
                    SysTick_CTRL_TICKINT_Msk   |    /* enable interrupt */
                    SysTick_CTRL_ENABLE_Msk;        /* enable SysTick */
}

void delay_ms(uint32_t ms) {
    delay_tick = ms;
    while (delay_tick != 0) {
    }
}
