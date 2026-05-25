#include "stm32f4xx.h"

void clock_init(void) {
    /* Enable HSI */
    RCC->CR |= RCC_CR_HSION;
    while ((RCC->CR & RCC_CR_HSIRDY) == 0);

    /* Enable HSE (25 Mhz) */
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) == 0);

    /* Setup PLL on 72 Mhz */
    RCC->PLLCFGR = (25 << 0) |                  // PLLM = 25
                   (144 << 6) |                 // PLLN = 144
                   (0 << 16) |                  // PLLP = 2 → 72 Mhz
                   (4 << 24) |                  // PLLQ = 4
                   RCC_PLLCFGR_PLLSRC_HSE;

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);

    FLASH->ACR = FLASH_ACR_PRFTEN | 
                 FLASH_ACR_ICEN | 
                 FLASH_ACR_DCEN | 
                 (2 << 0);                      // 2 wait states for 72 Mhz

    /* Setup bus dividers */
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;      // HCLK  = 72 Mhz
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;     // PCLK1 = 36 Mhz
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;     // PCLK2 = 72 Mhz

    /* Switch system clock to PLL */
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);

    /* update global variable */
    SystemCoreClock = 72000000UL;
}