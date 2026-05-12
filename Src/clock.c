#include "stm32f4xx.h"
#include "clock.h"

void clock_init(void){
    /* Enable HSE */
    RCC->CR |= RCC_CR_HSEON;
    while ((RCC->CR & RCC_CR_HSERDY) == 0);

    /* Setup PLL on 96 MHz */
    RCC->PLLCFGR = (25 << 0)
                    | (192 << 6)
                    | (0 << 16)
                    | (4 << 24)
                    | (RCC_PLLCFGR_PLLSRC_HSE);

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0);

    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | (3 << 0);

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;   /* HCLK = SYSCLK */
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;  /* PCLK1 = 48 МГц */
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;  /* PCLK2 = 96 МГц */

    /* Switch to PLL */
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL); 

    SystemCoreClock = 96000000UL;
}