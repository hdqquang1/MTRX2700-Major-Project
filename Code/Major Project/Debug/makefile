################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32F3xx_HAL_Driver/Src/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := Major Project
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
Major\ Project.elf \

MAP_FILES += \
Major\ Project.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
Major\ Project.list \


# All Target
all: main-build

# Main-build Target
main-build: Major\ Project.elf secondary-outputs

dependents:
	-cd /Users/jaydenwu/Documents/GitHub/MTRX2700-2024/STMF3disco-BSP/Debug && $(MAKE) all
	-cd /Users/jaydenwu/Documents/GitHub/MTRX2700-2024/stm32f303-definitions/Debug && $(MAKE) all

# Tool invocations
Major\ Project.elf Major\ Project.map: $(OBJS) $(USER_OBJS) /Users/jaydenwu/Documents/GitHub/MTRX2700-2024/STMF3disco-BSP/Debug/libSTMF3disco-BSP.a /Users/jaydenwu/Documents/GitHub/MTRX2700-2024/stm32f303-definitions/Debug/stm32f303-definitions /Users/jaydenwu/Downloads/MTRX2700-Major-Project-prototype/Major\ Project/STM32F303VCTX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "Major Project.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"/Users/jaydenwu/Downloads/MTRX2700-Major-Project-prototype/Major Project/STM32F303VCTX_FLASH.ld" --specs=nosys.specs -Wl,-Map="Major Project.map" -Wl,--gc-sections -static -L"/Users/jaydenwu/Documents/GitHub/MTRX2700-2024/STMF3disco-BSP/Debug" -u _printf_float --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

Major\ Project.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "Major Project.list"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) Major\ Project.elf Major\ Project.list Major\ Project.map Major\\ Project.elf default.size.stdout
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified
/Users/jaydenwu/Documents/GitHub/MTRX2700-2024/STMF3disco-BSP/Debug/libSTMF3disco-BSP.a:
/Users/jaydenwu/Documents/GitHub/MTRX2700-2024/stm32f303-definitions/Debug/stm32f303-definitions:

-include ../makefile.targets
