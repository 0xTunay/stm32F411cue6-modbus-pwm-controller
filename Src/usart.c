#include "usart.h"
#include "stm32f4xx.h"

void USART2_Init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    // PA2 TX, PA3 RX -> AF mode
    GPIOA->MODER &= ~((3U << (2 * 2)) | (3U << (3 * 2)));
    GPIOA->MODER |=  ((2U << (2 * 2)) | (2U << (3 * 2)));

    // AF7 USART2
    GPIOA->AFR[0] &= ~((0xF << (2 * 4)) | (0xF << (3 * 4)));
    GPIOA->AFR[0] |=  ((7U << (2 * 4)) | (7U << (3 * 4)));

    // Push-pull
    GPIOA->OTYPER &= ~((1U << 2) | (1U << 3));

    // High speed
    GPIOA->OSPEEDR |= ((3U << (2 * 2)) | (3U << (3 * 2)));

    // Pull-up RX
    GPIOA->PUPDR &= ~((3U << (2 * 2)) | (3U << (3 * 2)));
    GPIOA->PUPDR |=  (1U << (3 * 2));

    // 16MHz -> 115200
    // USART2->BRR = 0x8B;
    USART2->BRR = 0x0683; // 9600 baud for 96MHz
    USART2->CR1 = 0;
    USART2->CR1 |= USART_CR1_TE;
    USART2->CR1 |= USART_CR1_RE;
    USART2->CR1 |= USART_CR1_UE;
}
void USART2_SendChar(char ch)
{
    while(!(USART2->SR & USART_SR_TXE));

    USART2->DR = (uint8_t)ch;

    while(!(USART2->SR & USART_SR_TC));
}

void USART2_SendString(const char *str)
{
    while(*str)
    {
        USART2_SendChar(*str++);
    }
}

char USART2_GetChar(void)
{
    while(!(USART2->SR & USART_SR_RXNE));

    return (char)USART2->DR;
}