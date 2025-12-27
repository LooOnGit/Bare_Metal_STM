# FREERTOS_DIR = FreeRTOS-LTS/FreeRTOS/FreeRTOS-Kernel
# HEADER = -u _printf_float -specs=nano.specs -specs=nosys.specs -mfpu=fpv4-sp-d16 -mthumb -mfloat-abi=hard -mcpu=cortex-m4 -std=gnu11 -IDriver\Inc -I$(FREERTOS_DIR)/include/ -I. -I$(FREERTOS_DIR)/portable/GCC/ARM_CM4F

# Compiler flags for ARM Cortex-M4
CFLAGS = -mcpu=cortex-m4 -mthumb -std=gnu11 -O0
LDFLAGS = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -specs=nano.specs -specs=nosys.specs
BUILD_DIR = build


All:
	arm-none-eabi-gcc -c main.c $(CFLAGS) -o build/main.o
	arm-none-eabi-gcc -c  stm32f411_startup.c $(CFLAGS) -o build/startup.o
	arm-none-eabi-gcc -nostdlib -T stm32_ls.ld build/*.o -o build/bare_metal.elf
	arm-none-eabi-objcopy -O ihex build/bare_metal.elf build/bare_metal.hex
	arm-none-eabi-objcopy -O binary build/bare_metal.elf build/bare_metal.bin

$(BUILD_DIR):
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

Clean:
	if exist $(BUILD_DIR)\* del /Q $(BUILD_DIR)\*

Flash:
	STM32_Programmer_CLI -c port=SWD -w build/bare_metal.hex -v -s 0x08000000 -rst -run
