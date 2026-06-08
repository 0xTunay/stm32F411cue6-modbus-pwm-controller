#include "usart.h"
#include "stm32f4xx.h"

ModbusError_t USART2_Init(void) {
  MODBUS_ASSERT(GPIOA != NULL);
  MODBUS_ASSERT(USART2 != NULL);
  MODBUS_ASSERT(RCC != NULL);

  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

  /* configure PA2 (TX) and PA3 (RX) */
  GPIOA->MODER &= ~((3U << 4) | (3U << 6));
  GPIOA->MODER |=  ((2U << 4) | (2U << 6));

  /* set alternate function to AF7 (USART2) */
  GPIOA->AFR[0] &= ~((0xFU << 8) | (0xFU << 12));
  GPIOA->AFR[0] |=  ((7U << 8) | (7U << 12));

  /* set baud rate 9600 at 36 MHz PCLK1 */
  USART2->BRR = 0x0683;

  /* enable USART2: TX, RX, and USART module */
  USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;

  return MODBUS_OK;
}

ModbusError_t USART2_SendChar(char ch) {
  MODBUS_ASSERT(USART2 != NULL);

  uint32_t timeout = 1000000;
  while (!(USART2->SR & USART_SR_TXE) && timeout-- > 0);
  MODBUS_CHECK_COND(timeout > 0, MODBUS_ERR_TIMEOUT);

  USART2->DR = (uint8_t)ch;

  timeout = 1000000;
  while (!(USART2->SR & USART_SR_TC) && timeout-- > 0);
  MODBUS_CHECK_COND(timeout > 0, MODBUS_ERR_TIMEOUT);

  return MODBUS_OK;
}

ModbusError_t USART2_SendString(const char *str) {
  MODBUS_CHECK_NULL(str);
  MODBUS_ASSERT(USART2 != NULL);

  while (*str != '\0') {
    ModbusError_t err = USART2_SendChar(*str++);
    MODBUS_CHECK(err);
  }

  return MODBUS_OK;
}

ModbusError_t USART2_GetChar(char *ch) {
  MODBUS_CHECK_NULL(ch);
  MODBUS_ASSERT(USART2 != NULL);

  uint32_t timeout = 1000000;
  while (!(USART2->SR & USART_SR_RXNE) && timeout-- > 0);
  MODBUS_CHECK_COND(timeout > 0, MODBUS_ERR_TIMEOUT);

  *ch = (char)USART2->DR;

  return MODBUS_OK;
}

ModbusError_t USART2_GetCharTimeout(char *ch, uint32_t timeout_ms) {
  MODBUS_CHECK_NULL(ch);
  MODBUS_ASSERT(USART2 != NULL);

  uint32_t timeout = timeout_ms * 1000;
  while (!(USART2->SR & USART_SR_RXNE) && timeout-- > 0);
  MODBUS_CHECK_COND(timeout > 0, MODBUS_ERR_TIMEOUT);

  *ch = (char)USART2->DR;

  return MODBUS_OK;
}

ModbusError_t USART2_ReadLineTimeout(char *buf, uint32_t bufsize, uint32_t timeout_ms) {
  MODBUS_CHECK_NULL(buf);
  MODBUS_CHECK_COND(bufsize > 0, MODBUS_ERR_INVALID_PARAM);
  MODBUS_ASSERT(USART2 != NULL);

  uint32_t total_timeout = timeout_ms * 1000;
  uint32_t idx = 0;

  while (total_timeout-- > 0 && idx < bufsize - 1) {
    if (USART2->SR & USART_SR_RXNE) {
      char ch = (char)USART2->DR;
      if (ch == '\n') {
        buf[idx] = '\0';
        return MODBUS_OK;
      }
      buf[idx++] = ch;
    }
  }

  MODBUS_CHECK_COND(total_timeout > 0, MODBUS_ERR_TIMEOUT);
  buf[idx] = '\0';

  return MODBUS_OK;
}
