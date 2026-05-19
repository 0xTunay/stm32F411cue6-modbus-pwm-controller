#include "stm32f4xx.h"

void clock_init(void) {
    /* 1. enable HSE  */
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) == 0);

    /* 2. setup PLL on 48 МГц */
    RCC->PLLCFGR = (25 << 0) |           // PLLM = 25
                   (96 << 6) |           // PLLN = 96
                   (0 << 16) |           // PLLP = 2  → 48 МГц
                   (4 << 24) |           // PLLQ = 4
                   RCC_PLLCFGR_PLLSRC_HSE;

    /* 3. enable PLL */
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);

    /* 4. Flash latency (for 48 МГц enough 1 wait state) */
    FLASH->ACR = FLASH_ACR_PRFTEN | 
                 FLASH_ACR_ICEN | 
                 FLASH_ACR_DCEN | 
                 (1 << 0);               // 1 wait state

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;     // HCLK = 48 МГц
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV1;    // PCLK1 = 48 МГц
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;    // PCLK2 = 48 МГц

    /* 6. Переключаем SYSCLK на PLL */
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    SystemCoreClock = 48000000UL;
}