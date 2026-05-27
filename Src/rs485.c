#include "stm32f4xx.h"
#include "rs485.h"
#include "usart.h"
#include "stm32f4xx_registers.h"

void RS485_Init(void) {
    /* Enable GPIO clock for RS485 direction control pin */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    
    /* Configure PA4 (DE - Driver Enable) as GPIO output */
    RS485_DE_PORT->MODER &= ~(3U << (RS485_DE_PIN * 2));
    RS485_DE_PORT->MODER |=  (1U << (RS485_DE_PIN * 2));  // Output mode
    
    /* Configure as Push-Pull, High speed */
    RS485_DE_PORT->OTYPER  &= ~(1U << RS485_DE_PIN);      // Push-Pull
    RS485_DE_PORT->OSPEEDR |=  (3U << (RS485_DE_PIN * 2)); // High speed
    
    /* No pull-up/pull-down needed */
    RS485_DE_PORT->PUPDR &= ~(3U << (RS485_DE_PIN * 2));
    
    /* Initialize in RX mode (DE = 0) */
    RS485_DE_PORT->ODR &= ~(1U << RS485_DE_PIN);


    RS485_Receive_Enable();
    /* USART2 is already initialized by USART2_Init() */
}
void RS485_Transmit_Enable(void) {
    RS485_DE_PORT->BSRR = (1U << RS485_DE_PIN); // DE/RE = 1
}

void RS485_Receive_Enable(void) {
    RS485_DE_PORT->BSRR = (1U << (RS485_DE_PIN + 16)); // DE/RE = 0
}

void RS485_SendChar(char ch) {
    RS485_Transmit_Enable();
    
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = (uint8_t)ch;
    
    while (!(USART2->SR & USART_SR_TC));
    
    RS485_Receive_Enable();
}

void RS485_SendString(const char *str) {
    RS485_Transmit_Enable();
    
    while (*str)
    {
        while (!(USART2->SR & USART_SR_TXE));
        USART2->DR = (uint8_t)(*str++);
    }
    
    while (!(USART2->SR & USART_SR_TC));
    
    RS485_Receive_Enable();
}