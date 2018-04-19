################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../driver/source/m2m_ate_mode.c \
../driver/source/m2m_crypto.c \
../driver/source/m2m_hif.c \
../driver/source/m2m_ota.c \
../driver/source/m2m_periph.c \
../driver/source/m2m_ssl.c \
../driver/source/m2m_wifi.c \
../driver/source/nmasic.c \
../driver/source/nmbus.c \
../driver/source/nmdrv.c \
../driver/source/nmi2c.c \
../driver/source/nmspi.c \
../driver/source/nmuart.c 

OBJS += \
./driver/source/m2m_ate_mode.o \
./driver/source/m2m_crypto.o \
./driver/source/m2m_hif.o \
./driver/source/m2m_ota.o \
./driver/source/m2m_periph.o \
./driver/source/m2m_ssl.o \
./driver/source/m2m_wifi.o \
./driver/source/nmasic.o \
./driver/source/nmbus.o \
./driver/source/nmdrv.o \
./driver/source/nmi2c.o \
./driver/source/nmspi.o \
./driver/source/nmuart.o 

C_DEPS += \
./driver/source/m2m_ate_mode.d \
./driver/source/m2m_crypto.d \
./driver/source/m2m_hif.d \
./driver/source/m2m_ota.d \
./driver/source/m2m_periph.d \
./driver/source/m2m_ssl.d \
./driver/source/m2m_wifi.d \
./driver/source/nmasic.d \
./driver/source/nmbus.d \
./driver/source/nmdrv.d \
./driver/source/nmi2c.d \
./driver/source/nmspi.d \
./driver/source/nmuart.d 


# Each subdirectory must supply rules for building sources it contributes
driver/source/%.o: ../driver/source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F446xx -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/Inc" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/Drivers/CMSIS/Include" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/Inc" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/bsp/include" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/bus_wrapper/include" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/common/include" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/driver/include" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/driver/source" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/programmer" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/socket/include" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/spi_flash/include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


