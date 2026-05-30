#include "usart.h"
#include "stm32f4xx.h"

void USART2_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    GPIOA->MODER &= ~((3U << 4) | (3U << 6));
    GPIOA->MODER |=  ((2U << 4) | (2U << 6));

    GPIOA->AFR[0] &= ~((0xFU << 8) | (0xFU << 12));
    GPIOA->AFR[0] |=  ((7U << 8) | (7U << 12));

    USART2->BRR = 0x0683;

    USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void USART2_SendChar(char ch) {
    while (!(USART2->SR & USART_SR_TXE));   /* wait until buffer is empty */
    USART2->DR = (uint8_t)ch;

    while (!(USART2->SR & USART_SR_TC));    /* wait until transmission complete */
}

void USART2_SendString(const char *str) {
    while (*str)
    {
        USART2_SendChar(*str++);
    }
}

char USART2_GetChar(void) {
    while (!(USART2->SR & USART_SR_RXNE));  /* wait for received data */
    return (char)USART2->DR;
}