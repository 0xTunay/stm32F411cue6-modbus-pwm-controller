#ifndef RTCONFIG_H
#define RTCONFIG_H

/* Enable free modbus rtu slave mode */
#define PKG_MODBUS_SLAVE_RTU

/* slave register and coil mapping definitions */
#define RT_S_DISCRETE_INPUT_START      1
#define RT_S_DISCRETE_INPUT_NDISCRETES 16
#define RT_S_COIL_START                1
#define RT_S_COIL_NCOILS               16
#define RT_S_REG_INPUT_START           1
#define RT_S_REG_INPUT_NREGS           16
#define RT_S_REG_HOLDING_START         1
#define RT_S_REG_HOLDING_NREGS         16
#define RT_S_HD_RESERVE                0
#define RT_S_IN_RESERVE                0
#define RT_S_CO_RESERVE                0
#define RT_S_DI_RESERVE                0

/* master config defaults required by the library headers. */
#define RT_M_DISCRETE_INPUT_START      1
#define RT_M_DISCRETE_INPUT_NDISCRETES 16
#define RT_M_COIL_START                1
#define RT_M_COIL_NCOILS               16
#define RT_M_REG_INPUT_START           1
#define RT_M_REG_INPUT_NREGS           16
#define RT_M_REG_HOLDING_START         1
#define RT_M_REG_HOLDING_NREGS         16
#define RT_M_HD_RESERVE                0
#define RT_M_IN_RESERVE                0
#define RT_M_CO_RESERVE                0
#define RT_M_DI_RESERVE                0

#endif /* RTCONFIG_H */
