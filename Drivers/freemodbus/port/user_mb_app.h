#ifndef USER_APP
#define USER_APP
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbconfig.h"
#include "mbframe.h"
#include "mbutils.h"

/* -----------------------Slave Defines -------------------------------------*/
#define S_DISCRETE_INPUT_START      1
#define S_DISCRETE_INPUT_NDISCRETES 16
#define S_COIL_START                1
#define S_COIL_NCOILS               16
#define S_REG_INPUT_START           1
#define S_REG_INPUT_NREGS           16
#define S_REG_HOLDING_START         1
#define S_REG_HOLDING_NREGS         16
#define S_HD_RESERVE                0
#define S_IN_RESERVE                0
#define S_CO_RESERVE                0
#define S_DI_RESERVE                0

/* -----------------------Master Defines -------------------------------------*/
#define M_DISCRETE_INPUT_START      1
#define M_DISCRETE_INPUT_NDISCRETES 16
#define M_COIL_START                1
#define M_COIL_NCOILS               16
#define M_REG_INPUT_START           1
#define M_REG_INPUT_NREGS           16
#define M_REG_HOLDING_START         1
#define M_REG_HOLDING_NREGS         16
#define M_HD_RESERVE                0
#define M_IN_RESERVE                0
#define M_CO_RESERVE                0
#define M_DI_RESERVE                0

#endif
