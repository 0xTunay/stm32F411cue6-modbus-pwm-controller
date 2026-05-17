#include "usart.h"
#include "stm32f4xx.h"

void USART2_Init(void) {

    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    GPIOA->MODER &= ~(3U << 4);
    GPIOA->MODER |= (2U << 4);

    GPIOA->MODER &= ~(3U << 6);
    GPIOA->MODER |= (2U << 6);


    GPIOA->AFR[0] &= ~((0xF << 8) | (0xF << 12));
    GPIOA->AFR[0] |= ((7 << 8) | (7 << 12));


    USART2->BRR = 0x0341;

    /* Setup CR1 */

    USART2->CR1 = 0;
    USART2->CR1 |= (1 << 3); /* TE*/
    USART2->CR1 |= (1 << 2); /* RE */
    USART2->CR1 |= (1 << 13); /* UE */
}

void USART2_SendChar(char ch) {
    while(!(USART2->SR & USART_SR_TXE)); 
    USART2->DR = ch;

}

void USART2_SendString(const char *str){
 
    while(*str) {
      USART2_SendChar(*str++);
    }
}
char USART2_GetChar(void){
    while (!(USART2->SR & USART_SR_RXNE));
    return (char)USART2->DR;
}