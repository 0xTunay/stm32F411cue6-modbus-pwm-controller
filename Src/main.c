#include <stdint.h>
#include "stm32f4xx.h"
#include "clock.h"
#include "delay.h"
#include "usart.h"
#include "rs485.h"

int main(void) {
   // clock_init();
    SysTick_Init();
    RS485_Init();

    USART2_SendString("STM32F411 Modbus PWM Controller\r\n");
    USART2_SendString("System started successfully!\r\n");

    while(1) {
        char c = USART2_GetChar();
        USART2_SendChar(c);

        if (c == '\r')
            USART2_SendChar('\n');
    }
}