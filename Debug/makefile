################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F0xx_HAL_Driver/Src/subdir.mk
-include Core/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
para-biomedical.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
para-biomedical.list \

OBJCOPY_BIN += \
para-biomedical.bin \


# All Target
all: para-biomedical.elf secondary-outputs

# Tool invocations
para-biomedical.elf: $(OBJS) $(USER_OBJS)
	arm-none-eabi-gcc -o "para-biomedical.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0 --specs=nosys.specs -Wl,-Map="para-biomedical.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

para-biomedical.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "para-biomedical.list"
	@echo 'Finished building: $@'
	@echo ' '

para-biomedical.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "para-biomedical.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
