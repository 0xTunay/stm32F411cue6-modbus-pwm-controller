#include <stddef.h>
#include "stm32f4xx.h"
#include "port.h"
#include "mbport.h"
#include "usart.h"
#include "rs485.h"
#include "delay.h"
#include "error_handler.h"

static volatile uint8_t g_mb_event_head = 0;
static volatile uint8_t g_mb_event_tail = 0;
static volatile eMBEventType g_mb_event_queue[8];
static volatile uint32_t g_mb_timer_ticks = 0;
static volatile uint32_t g_mb_timer_reload = 0;
static volatile uint8_t g_mb_timer_enabled = FALSE;
static volatile BOOL g_mb_serial_rx_enabled = FALSE;
static volatile BOOL g_mb_serial_tx_enabled = FALSE;

void Modbus_TimerTick(void) {
  MODBUS_ASSERT(1);

  if (g_mb_timer_enabled && g_mb_timer_ticks > 0) {
    g_mb_timer_ticks--;
    if (g_mb_timer_ticks == 0) {
      g_mb_timer_enabled = FALSE;
      if (pxMBPortCBTimerExpired != NULL) {
        pxMBPortCBTimerExpired();
      }
    }
  }
}

void EnterCriticalSection(void) {
  MODBUS_ASSERT(1);
  __disable_irq();
}

void ExitCriticalSection(void) {
  MODBUS_ASSERT(1);
  __enable_irq();
}

BOOL xMBPortEventInit(void) {
  MODBUS_ASSERT(1);

  g_mb_event_head = 0;
  g_mb_event_tail = 0;
  return TRUE;
}

BOOL xMBPortEventPost(eMBEventType eEvent) {
  MODBUS_ASSERT(1);

  uint8_t nextTail = (uint8_t)((g_mb_event_tail + 1) & 0x07);
  if (nextTail == g_mb_event_head) {
    assert(0);
    return FALSE;
  }
  g_mb_event_queue[g_mb_event_tail] = eEvent;
  g_mb_event_tail = nextTail;
  return TRUE;
}

BOOL xMBPortEventGet(eMBEventType *eEvent) {
  MODBUS_ASSERT(eEvent != NULL);

  if (g_mb_event_head == g_mb_event_tail) {
    return FALSE;
  }
  *eEvent = g_mb_event_queue[g_mb_event_head];
  g_mb_event_head = (uint8_t)((g_mb_event_head + 1) & 0x07);
  return TRUE;
}

BOOL xMBPortSerialInit(UCHAR ucPort, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity) {
  (void) ucPort;
  (void) ulBaudRate;
  (void) ucDataBits;

  MODBUS_ASSERT(1);

  if (eParity != MB_PAR_NONE) {
    assert(0);
    return FALSE;
  }

  ModbusError_t err = USART2_Init();
  if (err != MODBUS_OK) {
    assert(0);
    return FALSE;
  }

  err = RS485_Init();
  if (err != MODBUS_OK) {
    assert(0);
    return FALSE;
  }

  g_mb_serial_rx_enabled = FALSE;
  g_mb_serial_tx_enabled = FALSE;
  return TRUE;
}

void xMBPortSerialClose(void) {
  MODBUS_ASSERT(1);
}

void vMBPortClose(void) {
  MODBUS_ASSERT(1);
}

void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable) {
  MODBUS_ASSERT(1);

  g_mb_serial_rx_enabled = xRxEnable;
  g_mb_serial_tx_enabled = xTxEnable;

  if (xTxEnable) {
    ModbusError_t err = RS485_Transmit_Enable();
    MODBUS_ASSERT(err == MODBUS_OK);
  } else {
    ModbusError_t err = RS485_Receive_Enable();
    MODBUS_ASSERT(err == MODBUS_OK);
  }
}

INLINE BOOL xMBPortSerialGetByte(CHAR * pucByte) {
  MODBUS_ASSERT(pucByte != NULL);

  if (pucByte == NULL) {
    assert(0);
    return FALSE;
  }
  *pucByte = (CHAR)USART2->DR;
  return TRUE;
}

INLINE BOOL xMBPortSerialPutByte(CHAR ucByte) {
  MODBUS_ASSERT(USART2 != NULL);

  uint32_t timeout = 1000000;
  while (!(USART2->SR & USART_SR_TXE) && timeout-- > 0);
  MODBUS_ASSERT(timeout > 0);

  USART2->DR = (uint8_t)ucByte;
  return TRUE;
}

BOOL xMBPortTimersInit(USHORT usTimeOut50us) {
  MODBUS_ASSERT(1);

  if (usTimeOut50us == 0) {
    assert(0);
    return FALSE;
  }

  uint32_t timeout_us = (uint32_t)usTimeOut50us * 50UL;
  g_mb_timer_reload = (timeout_us + 999U) / 1000U;
  if (g_mb_timer_reload == 0) {
    g_mb_timer_reload = 1;
  }

  g_mb_timer_enabled = FALSE;
  g_mb_timer_ticks = 0;
  return TRUE;
}

void xMBPortTimersClose(void) {
  MODBUS_ASSERT(1);
}

INLINE void vMBPortTimersEnable(void) {
  MODBUS_ASSERT(1);

  g_mb_timer_ticks = g_mb_timer_reload;
  g_mb_timer_enabled = TRUE;
}

INLINE void vMBPortTimersDisable(void) {
  MODBUS_ASSERT(1);

  g_mb_timer_enabled = FALSE;
}

void vMBPortSerialPoll(void) {
  MODBUS_ASSERT(USART2 != NULL);

  if (g_mb_serial_rx_enabled) {
    while (USART2->SR & USART_SR_RXNE) {
      if (pxMBFrameCBByteReceived != NULL) {
        pxMBFrameCBByteReceived();
      }
    }
  }

  if (g_mb_serial_tx_enabled && (USART2->SR & USART_SR_TXE)) {
    if (pxMBFrameCBTransmitterEmpty != NULL) {
      pxMBFrameCBTransmitterEmpty();
    }
  }
}
