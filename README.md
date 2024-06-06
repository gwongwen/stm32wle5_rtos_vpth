# Code for 6sens Omnitilt Project : test of STM32 ADC and BME280 sensor

## Overview
This first code allow us to convert a voltage and add processing to it:

 - take sample of battery level, temperature, humidity and pressure levels
 - store the different values in an area of partitioned internal memory (4kbytes at the end of flash memory)

This allows us to test BME280 ennvironmental sensor and the internal battery level ADC of the STM32WLE5xx. The final goal will be to send the samples to a lorawan server and clear the memory location once a day.

## Building and Running
The following commands clean build folder, build and flash the sample:

**Command to use**

west build -t pristine

west build -p always -b stm32wl_dw1000_iot_board applications/stm32wle5_rtos_sensor

west flash --runner stm32cubeprogrammer