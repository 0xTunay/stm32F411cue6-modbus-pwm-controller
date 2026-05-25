#include "usart.h"
#include "stm32f4xx.h"

void USART2_Init(void) {
    /* Enable tacks */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /* Setup PA2 (TX) and PA3 (RX) how Alternate Function */
    GPIOA->MODER &= ~((3U << 4) | (3U << 6));
    GPIOA->MODER |=  ((2U << 4) | (2U << 6));     // AF mode

    /* Choose AF7 for USART2 */
    GPIOA->AFR[0] &= ~((0xFU << 8) | (0xFU << 12));
    GPIOA->AFR[0] |=  ((7U << 8) | (7U << 12));

    /* Additional pin settings */
    GPIOA->OTYPER  &= ~((1U << 2) | (1U << 3));   // Push-Pull
    GPIOA->OSPEEDR |=  ((3U << 4) | (3U << 6));   // High speed
    GPIOA->PUPDR   &= ~(3U << 6);
    GPIOA->PUPDR   |=  (1U << 6);                 // Pull-up на RX

    USART2->BRR = 0x0271;        // 115200 baud @ 72 MHz

    /* Setup USART */
    USART2->CR1 = 0;
    USART2->CR1 |= USART_CR1_TE;     // Transmitter Enable
    USART2->CR1 |= USART_CR1_RE;     // Receiver Enable
    USART2->CR1 |= USART_CR1_UE;     // USART Enable
}

void USART2_SendChar(char ch) {
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = (uint8_t)ch;

    while (!(USART2->SR & USART_SR_TC));
}

void USART2_SendString(const char *str) {
    while (*str)
    {
        USART2_SendChar(*str++);
    }
}

char USART2_GetChar(void) {
    while (!(USART2->SR & USART_SR_RXNE));
    return (char)USART2->DR;
}