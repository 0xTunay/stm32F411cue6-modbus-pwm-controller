TARGET     = stm32f411-bare-metal
MCU        = STM32F411xE
CPU        = cortex-m4
FPU        = fpv4-sp-d16
FLOAT_ABI  = hard

CC      = arm-none-eabi-gcc
AS      = arm-none-eabi-gcc -x assembler-with-cpp
CP      = arm-none-eabi-objcopy
SZ      = arm-none-eabi-size
HEX     = $(CP) -O ihex
BIN     = $(CP) -O binary

SRC_DIR     = Src
INC_DIR     = Inc
CMSIS_DIR   = Drivers/CMSIS
VPATH       = Src:Drivers/freemodbus/modbus:Drivers/freemodbus/modbus/functions:Drivers/freemodbus/modbus/rtu:Drivers/freemodbus/port
C_SOURCES = \
$(SRC_DIR)/main.c \
$(SRC_DIR)/system_stm32f4xx.c \
$(SRC_DIR)/delay.c \
$(SRC_DIR)/usart.c \
$(SRC_DIR)/clock.c \
$(SRC_DIR)/pwm.c \
$(SRC_DIR)/rs485.c \
$(SRC_DIR)/modbus_port.c \
$(SRC_DIR)/syscalls.c \
Drivers/freemodbus/modbus/mb.c \
Drivers/freemodbus/modbus/mbutils.c \
Drivers/freemodbus/modbus/functions/mbfuncother.c \
Drivers/freemodbus/modbus/functions/mbfuncholding.c \
Drivers/freemodbus/modbus/functions/mbfuncinput.c \
Drivers/freemodbus/modbus/functions/mbfunccoils.c \
Drivers/freemodbus/modbus/functions/mbfuncdisc.c \
Drivers/freemodbus/modbus/rtu/mbcrc.c \
Drivers/freemodbus/modbus/rtu/mbrtu.c \
Drivers/freemodbus/port/user_mb_app.c

ASM_SOURCES = \
startup_stm32f411xe.s

INCLUDES = \
-I$(INC_DIR) \
-I$(CMSIS_DIR)/Device/ST/STM32F4xx/Include \
-I$(CMSIS_DIR)/Include \
-IDrivers/freemodbus/modbus/include \
-IDrivers/freemodbus/modbus/rtu \
-IDrivers/freemodbus/modbus/ascii \
-IDrivers/freemodbus/port

CFLAGS = -mcpu=$(CPU) \
         -mfpu=$(FPU) \
         -mfloat-abi=$(FLOAT_ABI) \
         -mthumb \
         -std=gnu11 \
         -Wall \
         -fdata-sections \
         -ffunction-sections \
         -g3 \
         -O0 \
         -D$(MCU) \
         -DUSE_FULL_ASSERT \
         $(INCLUDES)

LDFLAGS = -mcpu=$(CPU) \
          -mfpu=$(FPU) \
          -mfloat-abi=$(FLOAT_ABI) \
          -mthumb \
          -specs=nano.specs \
          -TSTM32F411XX_FLASH.ld \
          -Wl,--gc-sections \
          -Wl,-Map=$(TARGET).map,--cref

OBJECTS = $(patsubst %.c,build/%.o,$(notdir $(C_SOURCES))) \
          $(addprefix build/,$(notdir $(ASM_SOURCES:.s=.o)))

build/%.o: %.c | build
	mkdir -p $(dir $@)
	$(CC) -c $(CFLAGS) $< -o $@

all: $(TARGET).elf $(TARGET).hex $(TARGET).bin

build/%.o: %.s | build
	$(AS) -c $(CFLAGS) $< -o $@

$(TARGET).elf: $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	$(SZ) $@

$(TARGET).hex: $(TARGET).elf
	$(HEX) $< $@

$(TARGET).bin: $(TARGET).elf
	$(BIN) $< $@

build:
	mkdir -p build

clean:
	rm -rf build *.elf *.hex *.bin *.map

flash: $(TARGET).elf
	openocd -f openocd.cfg -c "program $(TARGET).elf verify reset exit"

.PHONY: all clean flash