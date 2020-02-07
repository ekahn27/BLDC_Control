################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/HallSensor.c \
../src/cr_startup_lpc15xx.c \
../src/crp.c \
../src/sysinit.c 

OBJS += \
./src/HallSensor.o \
./src/cr_startup_lpc15xx.o \
./src/crp.o \
./src/sysinit.o 

C_DEPS += \
./src/HallSensor.d \
./src/cr_startup_lpc15xx.d \
./src/crp.d \
./src/sysinit.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_LPCOPEN -D__LPC15XX__ -D__REDLIB__ -I"C:\Users\Eric\Documents\LPCXpresso_8.2.2_650\SEM-bldcControl\lpc_board_nxp_lpcxpresso_1549\inc" -I"C:\Users\Eric\Documents\LPCXpresso_8.2.2_650\SEM-bldcControl\lpc_chip_15xx\inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


