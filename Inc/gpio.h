/**
 * @file    gpio.h
 * @brief   GPIO low-level driver for STM32F411
 * @author  Loo
 * @version 1.0
 * @date    2026-01-08
 *
 * This driver provides basic GPIO control functions for PD12 (output) and PA0 (input).
 * It is implemented using CMSIS registers (no HAL dependency).
 */

#ifndef __GPIO__H__
#define __GPIO__H__

#define STM32F411xE
#include "stm32f4xx.h" 
#include "clock.h"
#include <stdbool.h>

/**
 * @defgroup GPIO GPIO Driver
 * @brief Low-level GPIO control for STM32F411
 * @{
 */

/** GPIO pin state: ON/SET */
#define ON     1U
/** GPIO pin state: OFF/RESET */
#define OFF    0U

/**
 * @brief Initialize GPIO pins
 * 
 * Configures PD12 as general purpose output and PA0 as input.
 * Enables required AHB1 clocks for GPIO ports.
 * 
 * @return None
 * 
 * @note PD12 is typically used for LED indicator
 * @see enableRccAHB1Clk()
 */
void initGPIOPin(void);

/**
 * @brief Write logic level to PD12 output pin
 * 
 * Sets or resets PD12 using ODR register (simple method, not atomic).
 * 
 * @param state Pin state: ON (1) = high, OFF (0) = low
 * 
 * @return None
 * 
 * @note Not atomic. Use writeGPIOPinBSSR() for interrupt-safe operation
 * @see writeGPIOPinBSSR()
 */
void writeGPIOPin(uint32_t state);

/**
 * @brief Write logic level to PD12 using BSRR register
 * 
 * Sets or resets PD12 using BSRR register (atomic operation).
 * 
 * @param state Pin state: ON (1) = high, OFF (0) = low
 * 
 * @return None
 * 
 * @note BSRR method is atomic - recommended for interrupt contexts
 * @see writeGPIOPin()
 */
void writeGPIOPinBSSR(uint32_t state);

/**
 * @brief Read logic level from PA0 input pin
 * 
 * Reads current state of PA0 from IDR register.
 * 
 * @param pin GPIO pin mask (currently unused, reads PA0)
 * 
 * @return Pin logic level (true = low, false = high)
 * 
 * @note Return value logic is inverted
 */
bool readGPIOPin(uint32_t pin);




#endif // __GPIO__H__
