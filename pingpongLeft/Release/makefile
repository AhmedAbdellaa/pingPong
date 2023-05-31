################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include 3-app/ping_pong/subdir.mk
-include 1-HAL/step_motor/subdir.mk
-include 1-HAL/SWITCH/subdir.mk
-include 1-HAL/SSD/subdir.mk
-include 1-HAL/LED/subdir.mk
-include 1-HAL/KEYPAD/subdir.mk
-include 1-HAL/DC_motor/subdir.mk
-include 1-HAL/CLCD/subdir.mk
-include 0-MCAL/4-GIE/subdir.mk
-include 0-MCAL/3-EXTI/subdir.mk
-include 0-MCAL/2-Port/subdir.mk
-include 0-MCAL/1-DIO/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
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

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
pingpongLeft.lss \

FLASH_IMAGE += \
pingpongLeft.hex \

EEPROM_IMAGE += \
pingpongLeft.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: pingpongLeft.elf secondary-outputs

# Tool invocations
pingpongLeft.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,pingpongLeft.map -mmcu=atmega32 -o "pingpongLeft.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

pingpongLeft.lss: pingpongLeft.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S pingpongLeft.elf  >"pingpongLeft.lss"
	@echo 'Finished building: $@'
	@echo ' '

pingpongLeft.hex: pingpongLeft.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex pingpongLeft.elf  "pingpongLeft.hex"
	@echo 'Finished building: $@'
	@echo ' '

pingpongLeft.eep: pingpongLeft.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex pingpongLeft.elf  "pingpongLeft.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: pingpongLeft.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega32 pingpongLeft.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) pingpongLeft.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
