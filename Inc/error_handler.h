#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

/* ModbusError_t enum for error handling */
typedef enum {
  MODBUS_OK = 0x00,               /* operation successful */
  MODBUS_ERR_INIT = 0x01,         /* initialization error */
  MODBUS_ERR_INVALID_PARAM = 0x02, /* invalid parameter */
  MODBUS_ERR_INVALID_ADDR = 0x03, /* invalid address or index */
  MODBUS_ERR_BUFFER_FULL = 0x04,  /* buffer is full */
  MODBUS_ERR_BUFFER_EMPTY = 0x05, /* buffer is empty */
  MODBUS_ERR_TIMEOUT = 0x06,      /* operation timeout */
  MODBUS_ERR_COMM = 0x07,         /* communication error */
  MODBUS_ERR_STATE = 0x08,        /* invalid state */
  MODBUS_ERR_CONFIG = 0x09,       /* configuration error */
  MODBUS_ERR_NULL_PTR = 0x0A,     /* NULL pointer error */
  MODBUS_ERR_UNKNOWN = 0xFF       /* unknown error */
} ModbusError_t;

/* assertion macro for error checking */
#define MODBUS_ASSERT(condition) assert(condition)

/* check and return macro */
#define MODBUS_CHECK(error) \
  do { \
    ModbusError_t err = (error); \
    if (err != MODBUS_OK) { \
      assert(0); \
      return err; \
    } \
  } while (0)

/* Check NULL pointer */
#define MODBUS_CHECK_NULL(ptr) \
  do { \
    if ((ptr) == NULL) { \
      assert(0); \
      return MODBUS_ERR_NULL_PTR; \
    } \
  } while (0)

/* check condition with error code */
#define MODBUS_CHECK_COND(condition, error_code) \
  do { \
    if (!(condition)) { \
      assert(0); \
      return (error_code); \
    } \
  } while (0)

#endif /* ERROR_HANDLER_H */
