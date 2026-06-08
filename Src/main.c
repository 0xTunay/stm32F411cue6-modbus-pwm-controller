#include <stdint.h>
#include "stm32f4xx.h"
#include "clock.h"
#include "delay.h"
#include "usart.h"
#include "rs485.h"
#include "port.h"
#include "pwm.h"
#include "mb.h"
#include "mbport.h"
#include "modbus_regs.h"
#include "error_handler.h"

int main(void) {
    ModbusError_t err;

    /* initialize clock */
    err = clock_init();
    if (err != MODBUS_OK) {
        assert(0);
        while (1) {
        }
    }

    /* initialize SysTick */
    err = SysTick_Init();
    if (err != MODBUS_OK) {
        assert(0);
        while (1) {
        }
    }

    /* initialize PWM */
    err = PWM_Init();
    if (err != MODBUS_OK) {
        assert(0);
        while (1) {
        }
    }

    /* initialize RS485 */
    err = RS485_Init();
    if (err != MODBUS_OK) {
        assert(0);
        while (1) {
        }
    }

    /* initialize Modbus registers */
    err = ModbusRegs_Init();
    if (err != MODBUS_OK) {
        assert(0);
        while (1) {
        }
    }

    /* send initial messages */
    err = USART2_SendString("STM32F411 Modbus RTU Slave starting...\r\n");
    if (err != MODBUS_OK) {
        assert(0);
        while (1) {
        }
    }

    err = USART2_SendString("PWM enabled on PA5 (TIM2_CH1) at 20 kHz, 50% duty.\r\n");
    if (err != MODBUS_OK) {
        assert(0);
        while (1) {
        }
    }

    /* initialize Modbus */
    if (eMBInit(MB_RTU, 1, 2, 9600, MB_PAR_NONE) != MB_ENOERR) {
        assert(0);
        USART2_SendString("Modbus init failed\r\n");
        while (1) {
        }
    }

    /* enable Modbus */
    if (eMBEnable() != MB_ENOERR) {
        assert(0);
        USART2_SendString("Modbus enable failed\r\n");
        while (1) {
        }
    }

    /* main loop */
    while (1) {
        vMBPortSerialPoll();
        eMBPoll();
    }

    return 0;
}
