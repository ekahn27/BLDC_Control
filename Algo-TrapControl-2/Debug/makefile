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
all: Algo-TrapControl-2.axf

# Tool invocations
Algo-TrapControl-2.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Eric\Documents\LPCXpresso\SEM-bldcControl\lpc_board_nxp_lpcxpresso_1549\Debug" -L"C:\Users\Eric\Documents\LPCXpresso\SEM-bldcControl\lpc_chip_15xx\Debug" -Xlinker -Map="Algo-TrapControl-2.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "Algo-TrapControl-2_Debug.ld" -o "Algo-TrapControl-2.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Algo-TrapControl-2.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Algo-TrapControl-2.axf"; # arm-none-eabi-objcopy -v -O binary "Algo-TrapControl-2.axf" "Algo-TrapControl-2.bin" ; # checksum -p LPC1549 -d "Algo-TrapControl-2.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
