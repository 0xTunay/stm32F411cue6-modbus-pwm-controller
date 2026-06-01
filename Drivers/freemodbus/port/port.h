// Drivers/freemodbus/port/port.h

#ifndef PORT_H
#define PORT_H

#include <stdint.h>
#include "stm32f411xe.h"

#define INLINE

typedef uint8_t     BOOL;
typedef uint8_t     UCHAR;
typedef int8_t      CHAR;
typedef uint16_t    USHORT;
typedef int16_t     SHORT;
typedef uint32_t    ULONG;
typedef int32_t     LONG;

#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif

#define ENTER_CRITICAL_SECTION()    __disable_irq()
#define EXIT_CRITICAL_SECTION()     __enable_irq()

#ifndef RT_ASSERT
#define RT_ASSERT(expr) ((void)0)
#endif

#define MB_PORT_HAS_CLOSE   0

void vMBPortSerialPoll(void);

#endif // PORT_H

