#include "rs485.h"
#include "usart.h"
#include "stm32f4xx.h"

#define RS485_EN_PIN     8

void RS485_Init(void)
{
    USART2_Init();

    /* Enable clock for GPIO */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /* Configure EN pin as output */
    GPIOA->MODER &= ~(3U << (RS485_EN_PIN * 2));
    GPIOA->MODER |=  (1U << (RS485_EN_PIN * 2));   // Output

    /* Start in receive mode */
    RS485_Receive_Enable();
}

void RS485_Transmit_Enable(void) {
    GPIOA->BSRR = (1U << RS485_EN_PIN);        // EN = 1 - Transmit
}

void RS485_Receive_Enable(void) {
    GPIOA->BSRR = (1U << (RS485_EN_PIN + 16)); // EN = 0 - receive
}

void RS485_SendChar(char ch) {
    RS485_Transmit_Enable();           // switch to transmit

    USART2_SendChar(ch);

    while (!(USART2->SR & USART_SR_TC));   // Wait until byte sent

    RS485_Receive_Enable();            // switch back to receive
}

void RS485_SendString(const char *str) {
    while (*str) {
        RS485_SendChar(*str++);
    }
}