#include <stdint.h>
#include "stm32f4xx.h"

#include "stm32f4xx_registers.h"
#include "delay.h"



int main(void) {

    SysTick_Init();
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~(3U << 24);
    GPIOA->MODER &= ~(3U << 26);
    GPIOA->MODER |=  (1U << 24);
    GPIOA->MODER |=  (1U << 26);

    while(1) {
        GPIOA->BSRR = BIT(28);
        GPIOA->BSRR = BIT(29);
        SysTick_Delay(100);

        GPIOA->BSRR = BIT(12);
        GPIOA->BSRR = BIT(13);
        SysTick_Delay(1000);
    }
}