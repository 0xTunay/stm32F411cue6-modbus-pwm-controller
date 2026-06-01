#include <stdint.h>
#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "rs485.h"
#include "port.h"
#include "mb.h"
#include "mbport.h"

int main(void) {
    SysTick_Init();
    RS485_Init();

    USART2_SendString("STM32F411 Modbus RTU Slave starting...\r\n");

    if (eMBInit(MB_RTU, 1, 2, 9600, MB_PAR_NONE) != MB_ENOERR) {
        USART2_SendString("Modbus init failed\r\n");
        while (1) {
        }
    }

    if (eMBEnable() != MB_ENOERR) {
        USART2_SendString("Modbus enable failed\r\n");
        while (1) {
        }
    }

    while (1) {
        vMBPortSerialPoll();
        eMBPoll();
    }
}
