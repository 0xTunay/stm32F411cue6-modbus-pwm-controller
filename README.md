# STM32F411CEU6 Modbus PWM Controller 
![MCU](https://img.shields.io/badge/MCU-STM32F411CEU6-03234B?logo=stmicroelectronics&logoColor=white)
![Build](https://img.shields.io/badge/build-cmake-064F8C?logo=cmake&logoColor=white)
![Protocol](https://img.shields.io/badge/protocol-Modbus-6C3483)

STM32F411CEU6 Modbus PWM Controller  its Bare-metal Modbus RTU slave implementation for STM32F411CEU6 with direct register-level peripheral control
I was started this project for get more expirience with bare metal programming, STM32F411CEU6 and Modbus protocol.

## Arhitecture System

This project consists of:
- STM32F411CEU6 microcontroller (ARM Cortex-M4 @ 100 MHz)
- MAX485 RS-485 transceiver module
- Three 12V PWM fans (4-pin with tachometer)
- USB-UART adapter for testing (CP2102)
- 12V 2A power supply

## System Architecture Diagram 

![img](docs/1777704624116.jpeg)

For more details, see the [STM32F411CE Documentation](https://www.st.com/resource/en/datasheet/stm32f411ce.pdf).
