#include <stdint.h>
#include "stm32f4xx.h"

#include "stm32f4xx_registers.h"
#include "delay.h"
#include "usart.h"
#include "clock.h"

int main(void) {
    
    clock_init();
    SysTick_Init();
    USART2_Init();

    USART2_SendString("Hello from STM32F411!\r\n");
    USART2_SendString("Echo test started...\r\n");

    
    while(1) {
        char c = USART2_GetChar();
        USART2_SendChar(c);
        if(c == '\r') {
            USART2_SendChar('\n');
        }
    }
}