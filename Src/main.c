#include <stdint.h>
#include "stm32f4xx.h"
#include "delay.h"
#include "usart.h"
#include "rs485.h"
#include "clock.h"
#include "mb.h"
#include "mbconfig.h"
#include "user_mb_app.h"

extern void vMBPortSerialPoll(void);

int main(void) {
    eMBErrorCode eStatus;

    clock_init();
    SysTick_Init();
    USART2_Init();
    RS485_Init();

    USART2_SendString("Modbus RTU slave starting...\r\n");

    eStatus = eMBInit(MB_RTU, 1, 1, 115200UL, MB_PAR_NONE);
    if (eStatus != MB_ENOERR) {
        USART2_SendString("Modbus init failed\r\n");
        while (1) {}
    }

    eStatus = eMBEnable();
    if (eStatus != MB_ENOERR) {
        USART2_SendString("Modbus enable failed\r\n");
        while (1) {}
    }

    USART2_SendString("Modbus RTU slave ready\r\n");

    while (1) {
        vMBPortSerialPoll();
        eMBPoll();
    }
}
