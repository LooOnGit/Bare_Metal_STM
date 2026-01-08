/**
 * @file    clock.h
 * @brief   Clock/RCC driver for STM32F411 - Peripheral Clock Control
 * @author  Loo
 * @version 1.0
 * @date    2026-01-08
 *
 * This driver provides functions to enable/disable peripheral clocks
 * on AHB1 and APB buses.
 */

#ifndef __CLOCK_H__
#define __CLOCK_H__

#define STM32F411xE
#include "stm32f4xx.h" 

/**
 * @defgroup CLOCK Clock/RCC Driver
 * @brief Reset and Clock Control for peripherals
 * @{
 */

/**
 * @brief Enable AHB1 peripheral clocks
 * 
 * Enables clock access for:
 * - GPIOD (GPIO Port D - for PD12 output)
 * - GPIOA (GPIO Port A - for PA0 input and UART2)
 * 
 * @return None
 * 
 * @note Must be called before accessing GPIO registers
 */
void enableRccAHB1Clk(void);

/** @} */"

#endif // __CLOCK_H__