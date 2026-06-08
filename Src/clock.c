#include "stm32f4xx.h"
#include "error_handler.h"

ModbusError_t clock_init(void) {
    /* enable HSI as backup clock */
    RCC->CR |= RCC_CR_HSION;
    uint32_t timeout = 1000000;
    while ((RCC->CR & RCC_CR_HSIRDY) == 0 && timeout-- > 0);
    MODBUS_CHECK_COND(timeout > 0, MODBUS_ERR_INIT);

    /* enable HSE (25 MHz crystal) */
    RCC->CR |= RCC_CR_HSEON;
    timeout = 1000000;
    while ((RCC->CR & RCC_CR_HSERDY) == 0 && timeout-- > 0);
    MODBUS_CHECK_COND(timeout > 0, MODBUS_ERR_INIT);

    /* configure PLL for 72 MHz */
    RCC->PLLCFGR = (25 << 0) |           /* PLLM = 25 */
                   (144 << 6) |          /* PLLN = 144 */
                   (0 << 16) |           /* PLLP = 2 */
                   (4 << 24) |           /* PLLQ = 4 */
                   RCC_PLLCFGR_PLLSRC_HSE;

    /* enable PLL */
    RCC->CR |= RCC_CR_PLLON;
    timeout = 1000000;
    while ((RCC->CR & RCC_CR_PLLRDY) == 0 && timeout-- > 0);
    MODBUS_CHECK_COND(timeout > 0, MODBUS_ERR_INIT);

    /* set Flash latency, important for high speed */
    FLASH->ACR = FLASH_ACR_PRFTEN | 
                 FLASH_ACR_ICEN | 
                 FLASH_ACR_DCEN | 
                 (2 << 0);               /* 2 wait states for 72 MHz */

    /* configure bus prescalers */
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;     /* HCLK = 72 MHz */
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;    /* PCLK1 = 36 MHz */
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;    /* PCLK2 = 72 MHz */

    /* switch system clock to PLL */
    RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | RCC_CFGR_SW_PLL;
    timeout = 1000000;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL && timeout-- > 0);
    MODBUS_CHECK_COND(timeout > 0, MODBUS_ERR_INIT);

    SystemCoreClock = 72000000UL;
    
    return MODBUS_OK;
}
