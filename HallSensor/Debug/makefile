################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: HallSensor.axf

# Tool invocations
HallSensor.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Eric\Documents\LPCXpresso_8.2.2_650\SEM-bldcControl\lpc_board_nxp_lpcxpresso_1549\Debug" -L"C:\Users\Eric\Documents\LPCXpresso_8.2.2_650\SEM-bldcControl\lpc_chip_15xx\Debug" -Xlinker -Map="HallSensor.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "HallSensor_Debug.ld" -o "HallSensor.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) HallSensor.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "HallSensor.axf"; # arm-none-eabi-objcopy -v -O binary "HallSensor.axf" "HallSensor.bin" ; # checksum -p LPC1549 -d "HallSensor.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
