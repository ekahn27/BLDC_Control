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
all: Copy5-adcRom.axf

# Tool invocations
Copy5-adcRom.axf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Eric\Documents\LPCXpresso\SEM-bldcControl\lpc_board_nxp_lpcxpresso_1549\Debug" -L"C:\Users\Eric\Documents\LPCXpresso\SEM-bldcControl\lpc_chip_15xx\Debug" -Xlinker -Map="Copy5-adcRom.map" -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m3 -mthumb -T "Copy5-adcRom_Debug.ld" -o "Copy5-adcRom.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(EXECUTABLES)$(OBJS)$(C_DEPS) Copy5-adcRom.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Copy5-adcRom.axf"; # arm-none-eabi-objcopy -v -O binary "Copy5-adcRom.axf" "Copy5-adcRom.bin" ; # checksum -p LPC1549 -d "Copy5-adcRom.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
