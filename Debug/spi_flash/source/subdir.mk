################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../spi_flash/source/spi_flash.c 

OBJS += \
./spi_flash/source/spi_flash.o 

C_DEPS += \
./spi_flash/source/spi_flash.d 


# Each subdirectory must supply rules for building sources it contributes
spi_flash/source/%.o: ../spi_flash/source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F446xx -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/Inc" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/Drivers/STM32F4xx_HAL_Driver/Inc" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/Drivers/CMSIS/Device/ST/STM32F4xx/Include" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/Drivers/CMSIS/Include" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/Inc" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/bsp/include" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/bus_wrapper/include" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/common/include" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/driver/include" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/driver/source" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/programmer" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/socket/include" -I"/Users/brett-mac/Documents/tech/workspace_c/cubeEx/stmWifi/spi_flash/include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


