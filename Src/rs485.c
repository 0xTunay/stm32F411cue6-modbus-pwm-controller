#include "rs485.h"
#include "usart.h"
#include "stm32f4xx.h"

#define RS485_EN_PIN     8

ModbusError_t RS485_Init(void) {
    MODBUS_ASSERT(RCC != NULL);
    MODBUS_ASSERT(GPIOA != NULL);

    /* initialize USART first */
    ModbusError_t err = USART2_Init();
    MODBUS_CHECK(err);

    /* enable clock for GPIO */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* configure EN pin (PA8) as output */
    GPIOA->MODER &= ~(3U << (RS485_EN_PIN * 2));
    GPIOA->MODER |=  (1U << (RS485_EN_PIN * 2));

    /* start in receive mode */
    err = RS485_Receive_Enable();
    MODBUS_CHECK(err);

    return MODBUS_OK;
}

ModbusError_t RS485_Transmit_Enable(void) {
    MODBUS_ASSERT(GPIOA != NULL);

    GPIOA->BSRR = (1U << RS485_EN_PIN);

    return MODBUS_OK;
}

ModbusError_t RS485_Receive_Enable(void) {
    MODBUS_ASSERT(GPIOA != NULL);

    GPIOA->BSRR = (1U << (RS485_EN_PIN + 16));

    return MODBUS_OK;
}

ModbusError_t RS485_SendChar(char ch) {
    MODBUS_ASSERT(GPIOA != NULL);
    MODBUS_ASSERT(USART2 != NULL);

    /* switch to transmit mode */
    ModbusError_t err = RS485_Transmit_Enable();
    MODBUS_CHECK(err);

    /* send character */
    err = USART2_SendChar(ch);
    MODBUS_CHECK(err);

    /* wait until transmission is complete */
    uint32_t timeout = 1000000;
    while (!(USART2->SR & USART_SR_TC) && timeout-- > 0);
    MODBUS_CHECK_COND(timeout > 0, MODBUS_ERR_TIMEOUT);

    /* switch back to receive mode */
    err = RS485_Receive_Enable();
    MODBUS_CHECK(err);

    return MODBUS_OK;
}

ModbusError_t RS485_SendString(const char *str) {
    MODBUS_CHECK_NULL(str);
    MODBUS_ASSERT(GPIOA != NULL);
    MODBUS_ASSERT(USART2 != NULL);

    while (*str != '\0') {
        ModbusError_t err = RS485_SendChar(*str++);
        MODBUS_CHECK(err);
    }

    return MODBUS_OK;
}
