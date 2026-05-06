#include "stm32f4xx.h"

uint32_t SystemCoreClock = 16000000UL;   // Default HSI 16 MHz

/**
 * @brief  Setup the microcontroller system
 *         Initialize the FPU, vector table and system clock
 */
void SystemInit(void)
{
    /* FPU settings ------------------------------------------------------------*/
    #if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
        SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
    #endif

    /* Reset the RCC clock configuration to the default reset state */
    /* Set HSION bit */
    RCC->CR |= (uint32_t)0x00000001;

    /* Reset CFGR register */
    RCC->CFGR = 0x00000000;

    /* Reset HSEON, CSSON and PLLON bits */
    RCC->CR &= (uint32_t)0xFEF6FFFF;

    /* Reset PLLCFGR register */
    RCC->PLLCFGR = 0x24003010;

    /* Reset HSEBYP bit */
    RCC->CR &= (uint32_t)0xFFFBFFFF;

    /* Disable all interrupts */
    RCC->CIR = 0x00000000;
}
