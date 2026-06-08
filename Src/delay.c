#include "delay.h"
#include "stm32f4xx.h"
#include "error_handler.h"

extern void Modbus_TimerTick(void);

static volatile uint32_t delay_tick = 0;

void SysTick_Handler(void) {
  MODBUS_ASSERT(SysTick != NULL);

  if (delay_tick > 0) {
    delay_tick--;
  }
  Modbus_TimerTick();
}

ModbusError_t SysTick_Init(void) {
  MODBUS_ASSERT(SysTick != NULL);
  MODBUS_ASSERT(SystemCoreClock > 0);

  if (SystemCoreClock == 0) {
    assert(0);
    return MODBUS_ERR_CONFIG;
  }

  /* system core clock is set in clock_init() */
  SysTick->LOAD = (SystemCoreClock / 1000) - 1;
  MODBUS_CHECK_COND(SysTick->LOAD > 0, MODBUS_ERR_CONFIG);

  SysTick->VAL  = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
    SysTick_CTRL_TICKINT_Msk   |
    SysTick_CTRL_ENABLE_Msk;

  return MODBUS_OK;
}

ModbusError_t SysTick_Delay(uint32_t ms) {
  MODBUS_ASSERT(SysTick != NULL);

  delay_tick = ms;
  while (delay_tick != 0) {
  }

  return MODBUS_OK;
}
