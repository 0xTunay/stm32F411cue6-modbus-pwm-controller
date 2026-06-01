/*
 * FreeModbus Libary: Modbus configuration for STM32F411 bare-metal
 */

#ifndef _MB_CONFIG_H
#define _MB_CONFIG_H

#include <stdint.h>

/* Slave mode only */
#define MB_MASTER_ASCII_ENABLED                0
#define MB_MASTER_RTU_ENABLED                  0
#define MB_MASTER_TCP_ENABLED                  0

#define MB_SLAVE_ASCII_ENABLED                 0
#define MB_SLAVE_RTU_ENABLED                   1
#define MB_SLAVE_TCP_ENABLED                   0

#define MB_ASCII_TIMEOUT_SEC                   1
#define MB_FUNC_HANDLERS_MAX                   16
#define MB_FUNC_OTHER_REP_SLAVEID_BUF          32
#define MB_FUNC_OTHER_REP_SLAVEID_ENABLED      1

#define MB_FUNC_READ_INPUT_ENABLED             1
#define MB_FUNC_READ_HOLDING_ENABLED           1
#define MB_FUNC_WRITE_HOLDING_ENABLED          1
#define MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED 1
#define MB_FUNC_READ_COILS_ENABLED             1
#define MB_FUNC_WRITE_COIL_ENABLED             1
#define MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED   1
#define MB_FUNC_READ_DISCRETE_INPUTS_ENABLED   1
#define MB_FUNC_READWRITE_HOLDING_ENABLED      1

#define MB_PDU_SIZE_MAX                        253
#define MB_SER_PDU_SIZE_MAX                    256

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
#define MB_MASTER_DELAY_MS_CONVERT             200
#define MB_MASTER_TIMEOUT_MS_RESPOND           100
#define MB_MASTER_TOTAL_SLAVE_NUM              16
#endif

#endif /* _MB_CONFIG_H */
