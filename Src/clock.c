/**
 * @file    clock.c
 * @brief   Clock/RCC driver implementation for STM32F411
 * @author  Loo
 * @version 1.0
 * @date    2026-01-08
 */

#include "clock.h"

/**
 * @brief Enable AHB1 peripheral clocks
 * 
 * Enables clock access for GPIOD and GPIOA ports.
 * 
 * @return None
 * 
 * @note Should be called during initialization before GPIO configuration
 */
void enableRccAHB1Clk(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // Enable GPIOD clock (bit 3)

    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Enable GPIOD clock (bit 3)
}