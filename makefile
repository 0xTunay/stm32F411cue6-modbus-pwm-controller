.PHONY: build flash clean all rebuild reset

ELF=build/Debug/stm32F411cue6-modbus-pwm-controller.elf
OPENOCD_CFG=openocd.cfg

build:
	cmake --build --preset Debug

flash: build
	openocd -f $(OPENOCD_CFG) -c "program $(ELF) verify reset exit"

all: flash

clean:
	rm -rf build/Debug/*

rebuild: clean build

reset:
	openocd -f $(OPENOCD_CFG) -c "init; reset; exit"