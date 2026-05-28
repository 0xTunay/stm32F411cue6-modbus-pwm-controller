#include <stddef.h>
#include "stm32f4xx.h"
#include "port.h"
#include "mbport.h"
#include "usart.h"
#include "rs485.h"
#include "delay.h"

static volatile uint8_t g_mb_event_head = 0;
static volatile uint8_t g_mb_event_tail = 0;
static volatile eMBEventType g_mb_event_queue[8];
static volatile uint32_t g_mb_timer_ticks = 0;
static volatile uint32_t g_mb_timer_reload = 0;
static volatile uint8_t g_mb_timer_enabled = FALSE;
static volatile BOOL g_mb_serial_rx_enabled = FALSE;
static volatile BOOL g_mb_serial_tx_enabled = FALSE;

void Modbus_TimerTick(void) {
    if (g_mb_timer_enabled && g_mb_timer_ticks > 0) {
        g_mb_timer_ticks--;
        if (g_mb_timer_ticks == 0) {
            g_mb_timer_enabled = FALSE;
            if (pxMBPortCBTimerExpired != NULL){
                pxMBPortCBTimerExpired();
            }
        }
    }
}

void EnterCriticalSection(void) {
    __disable_irq();
}

void ExitCriticalSection(void){
    __enable_irq();
}

BOOL xMBPortEventInit(void) {
    g_mb_event_head = 0;
    g_mb_event_tail = 0;
    return TRUE;
}

BOOL xMBPortEventPost(eMBEventType eEvent) {
    uint8_t nextTail = (uint8_t)((g_mb_event_tail + 1) & 0x07);
    if (nextTail == g_mb_event_head) {
        return FALSE;
    }
    g_mb_event_queue[g_mb_event_tail] = eEvent;
    g_mb_event_tail = nextTail;
    return TRUE;
}

BOOL xMBPortEventGet(eMBEventType *eEvent) {
    if (g_mb_event_head == g_mb_event_tail){
        return FALSE;
    }
    *eEvent = g_mb_event_queue[g_mb_event_head];
    g_mb_event_head = (uint8_t)((g_mb_event_head + 1) & 0x07);
    return TRUE;
}

BOOL xMBPortSerialInit(UCHAR ucPort, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity) {
    (void) ucPort;
    (void) ucDataBits;

    if (eParity != MB_PAR_NONE) {
        return FALSE;
    }

    USART2_Init();
    RS485_Init();
    g_mb_serial_rx_enabled = FALSE;
    g_mb_serial_tx_enabled = FALSE;
    return TRUE;
}

void xMBPortSerialClose(void) {}

void vMBPortClose(void) {}

void vMBPortSerialEnable(BOOL xRxEnable, BOOL xTxEnable) {
    g_mb_serial_rx_enabled = xRxEnable;
    g_mb_serial_tx_enabled = xTxEnable;
    if (xTxEnable){
        RS485_Transmit_Enable();
    } else {
        RS485_Receive_Enable();
    }
}

INLINE BOOL xMBPortSerialGetByte(CHAR * pucByte)
{
    if (pucByte == NULL)
    {
        return FALSE;
    }
    *pucByte = (CHAR)USART2->DR;
    return TRUE;
}

INLINE BOOL xMBPortSerialPutByte(CHAR ucByte) {
    while (!(USART2->SR & USART_SR_TXE)) 
    {}
    USART2->DR = (uint8_t)ucByte;
    return TRUE;
}

BOOL xMBPortTimersInit(USHORT usTimeOut50us) {
    if (usTimeOut50us == 0) {
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

void xMBPortTimersClose(void) {}

INLINE void vMBPortTimersEnable(void) {
    g_mb_timer_ticks = g_mb_timer_reload;
    g_mb_timer_enabled = TRUE;
}

INLINE void vMBPortTimersDisable(void) {
    g_mb_timer_enabled = FALSE;
}

void vMBPortSerialPoll(void) {
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
