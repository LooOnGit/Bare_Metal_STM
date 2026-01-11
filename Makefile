# Compiler and flags
CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy
CFLAGS = -mcpu=cortex-m4 -mthumb -std=gnu11 -O0 #compliation flags
LDFLAGS = -T stm32_ls.ld -nostdlib  # linker flags
BUILD_DIR = build

# Include paths
INCLUDES = -I Header_CMSIS \
           -I Header_CMSIS/STM32CubeF4/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
           -I Header_CMSIS/STM32CubeF4/Drivers/CMSIS/Include \
           -I Header_CMSIS/STM32CubeF4/Drivers/STM32F4xx_HAL_Driver/Inc \
		   -I Inc/ \

# Targets
All: $(BUILD_DIR)
	$(CC) -c src/main.c $(CFLAGS) $(INCLUDES) -o $(BUILD_DIR)/main.o
	$(CC) -c startup/stm32f411_startup.c $(CFLAGS) $(INCLUDES) -o $(BUILD_DIR)/startup.o
	$(CC) -c src/gpio.c $(CFLAGS) $(INCLUDES) -o $(BUILD_DIR)/gpio.o
	$(CC) -c src/clock.c $(CFLAGS) $(INCLUDES) -o $(BUILD_DIR)/clock.o
	$(CC) -c src/systick.c $(CFLAGS) $(INCLUDES) -o $(BUILD_DIR)/systick.o
	$(CC) -c src/timer.c $(CFLAGS) $(INCLUDES) -o $(BUILD_DIR)/timer.o
	$(CC) -c src/uart.c $(CFLAGS) $(INCLUDES) -o $(BUILD_DIR)/uart.o
	$(CC) -c src/adc.c $(CFLAGS) $(INCLUDES) -o $(BUILD_DIR)/adc.o
	$(CC) -c src/spi.c $(CFLAGS) $(INCLUDES) -o $(BUILD_DIR)/spi.o
	$(CC) $(LDFLAGS) $(BUILD_DIR)/*.o -o $(BUILD_DIR)/bare_metal.elf
	$(OBJCOPY) -O ihex $(BUILD_DIR)/bare_metal.elf $(BUILD_DIR)/bare_metal.hex
	$(OBJCOPY) -O binary $(BUILD_DIR)/bare_metal.elf $(BUILD_DIR)/bare_metal.bin

$(BUILD_DIR):
	if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

Clean:
	if exist $(BUILD_DIR)\* del /Q $(BUILD_DIR)\*

Flash:
	STM32_Programmer_CLI -c port=SWD -w build/bare_metal.hex -v -s 0x08000000 -rst -run