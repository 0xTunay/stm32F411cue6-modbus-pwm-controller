#include "modbus_regs.h"
#include <string.h>

static uint16_t s_holding_regs[MODBUS_HOLDING_COUNT];
static uint16_t s_input_regs[MODBUS_INPUT_COUNT];
static uint8_t s_coil_buf[(MODBUS_COIL_COUNT + 7) / 8];
static uint8_t s_discrete_buf[(MODBUS_DISCRETE_COUNT + 7) / 8];

static inline bool ModbusRegs_IndexValid(uint16_t index, uint16_t count) {
  return index < count;
}

static inline uint16_t ModbusRegs_BitByteIndex(uint16_t index) {
  return index / 8u;
}

static inline uint8_t ModbusRegs_BitMask(uint16_t index) {
  return (uint8_t)(1u << (index % 8u));
}

ModbusError_t ModbusRegs_Init(void) {
  MODBUS_ASSERT(s_holding_regs != NULL);
  MODBUS_ASSERT(s_input_regs != NULL);
  MODBUS_ASSERT(s_coil_buf != NULL);
  MODBUS_ASSERT(s_discrete_buf != NULL);

  return ModbusRegs_Reset();
}

ModbusError_t ModbusRegs_Reset(void) {
  MODBUS_ASSERT(s_holding_regs != NULL);
  MODBUS_ASSERT(s_input_regs != NULL);
  MODBUS_ASSERT(s_coil_buf != NULL);
  MODBUS_ASSERT(s_discrete_buf != NULL);

  memset(s_holding_regs, 0, sizeof(s_holding_regs));
  memset(s_input_regs, 0, sizeof(s_input_regs));
  memset(s_coil_buf, 0, sizeof(s_coil_buf));
  memset(s_discrete_buf, 0, sizeof(s_discrete_buf));

  ModbusError_t err;

  err = ModbusRegs_SetHolding(MODBUS_HOLDING_REG_PWM_DUTY, 5000);
  MODBUS_CHECK(err);

  err = ModbusRegs_SetHolding(MODBUS_HOLDING_REG_PWM_FREQUENCY, 20000);
  MODBUS_CHECK(err);

  err = ModbusRegs_SetHolding(MODBUS_HOLDING_REG_STATUS, 1);
  MODBUS_CHECK(err);

  err = ModbusRegs_SetInput(MODBUS_INPUT_REG_DEVICE_ID, 0xF411);
  MODBUS_CHECK(err);

  err = ModbusRegs_SetInput(MODBUS_INPUT_REG_FIRMWARE_MAJOR, 1);
  MODBUS_CHECK(err);

  err = ModbusRegs_SetInput(MODBUS_INPUT_REG_FIRMWARE_MINOR, 0);
  MODBUS_CHECK(err);

  err = ModbusRegs_SetDiscrete(MODBUS_DISCRETE_READY, true);
  MODBUS_CHECK(err);

  err = ModbusRegs_SetCoil(MODBUS_COIL_PWM_ENABLE, true);
  MODBUS_CHECK(err);

  return MODBUS_OK;
}

uint16_t *ModbusRegs_GetHoldingBuffer(void) {
  MODBUS_ASSERT(s_holding_regs != NULL);
  return s_holding_regs;
}

uint16_t *ModbusRegs_GetInputBuffer(void) {
  MODBUS_ASSERT(s_input_regs != NULL);
  return s_input_regs;
}

uint8_t *ModbusRegs_GetCoilBuffer(void) {
  MODBUS_ASSERT(s_coil_buf != NULL);
  return s_coil_buf;
}

uint8_t *ModbusRegs_GetDiscreteInputBuffer(void) {
  MODBUS_ASSERT(s_discrete_buf != NULL);
  return s_discrete_buf;
}

ModbusError_t ModbusRegs_GetHolding(uint16_t index, uint16_t *value) {
  MODBUS_CHECK_NULL(value);
  MODBUS_ASSERT(s_holding_regs != NULL);
  MODBUS_CHECK_COND(ModbusRegs_IndexValid(index, MODBUS_HOLDING_COUNT), 
      MODBUS_ERR_INVALID_ADDR);

  *value = s_holding_regs[index];
  return MODBUS_OK;
}

ModbusError_t ModbusRegs_SetHolding(uint16_t index, uint16_t value) {
  MODBUS_ASSERT(s_holding_regs != NULL);
  MODBUS_CHECK_COND(ModbusRegs_IndexValid(index, MODBUS_HOLDING_COUNT), 
      MODBUS_ERR_INVALID_ADDR);

  s_holding_regs[index] = value;
  return MODBUS_OK;
}

ModbusError_t ModbusRegs_GetInput(uint16_t index, uint16_t *value) {
  MODBUS_CHECK_NULL(value);
  MODBUS_ASSERT(s_input_regs != NULL);
  MODBUS_CHECK_COND(ModbusRegs_IndexValid(index, MODBUS_INPUT_COUNT), 
      MODBUS_ERR_INVALID_ADDR);

  *value = s_input_regs[index];
  return MODBUS_OK;
}

ModbusError_t ModbusRegs_SetInput(uint16_t index, uint16_t value) {
  MODBUS_ASSERT(s_input_regs != NULL);
  MODBUS_CHECK_COND(ModbusRegs_IndexValid(index, MODBUS_INPUT_COUNT), 
      MODBUS_ERR_INVALID_ADDR);

  s_input_regs[index] = value;
  return MODBUS_OK;
}

ModbusError_t ModbusRegs_GetCoil(uint16_t index, bool *state) {
  MODBUS_CHECK_NULL(state);
  MODBUS_ASSERT(s_coil_buf != NULL);
  MODBUS_CHECK_COND(ModbusRegs_IndexValid(index, MODBUS_COIL_COUNT), 
      MODBUS_ERR_INVALID_ADDR);

  uint16_t byteIndex = ModbusRegs_BitByteIndex(index);
  uint8_t mask = ModbusRegs_BitMask(index);
  *state = (s_coil_buf[byteIndex] & mask) != 0u;

  return MODBUS_OK;
}

ModbusError_t ModbusRegs_SetCoil(uint16_t index, bool state) {
  MODBUS_ASSERT(s_coil_buf != NULL);
  MODBUS_CHECK_COND(ModbusRegs_IndexValid(index, MODBUS_COIL_COUNT), 
      MODBUS_ERR_INVALID_ADDR);

  uint16_t byteIndex = ModbusRegs_BitByteIndex(index);
  uint8_t mask = ModbusRegs_BitMask(index);

  if (state) {
    s_coil_buf[byteIndex] |= mask;
  } else {
    s_coil_buf[byteIndex] &= (uint8_t)~mask;
  }

  return MODBUS_OK;
}

ModbusError_t ModbusRegs_GetDiscrete(uint16_t index, bool *state) {
  MODBUS_CHECK_NULL(state);
  MODBUS_ASSERT(s_discrete_buf != NULL);
  MODBUS_CHECK_COND(ModbusRegs_IndexValid(index, MODBUS_DISCRETE_COUNT), 
      MODBUS_ERR_INVALID_ADDR);

  uint16_t byteIndex = ModbusRegs_BitByteIndex(index);
  uint8_t mask = ModbusRegs_BitMask(index);
  *state = (s_discrete_buf[byteIndex] & mask) != 0u;

  return MODBUS_OK;
}

ModbusError_t ModbusRegs_SetDiscrete(uint16_t index, bool state) {
  MODBUS_ASSERT(s_discrete_buf != NULL);
  MODBUS_CHECK_COND(ModbusRegs_IndexValid(index, MODBUS_DISCRETE_COUNT), 
      MODBUS_ERR_INVALID_ADDR);

  uint16_t byteIndex = ModbusRegs_BitByteIndex(index);
  uint8_t mask = ModbusRegs_BitMask(index);

  if (state) {
    s_discrete_buf[byteIndex] |= mask;
  } else {
    s_discrete_buf[byteIndex] &= (uint8_t)~mask;
  }

  return MODBUS_OK;
}
