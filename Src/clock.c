#include "clock.h"

/**
 * @defgroup RCC_Driver RCC Driver
 * @brief Reset and Clock Control driver for peripheral clocks
 * @{
 */

/**
 * @defgroup RCC_AHB1_Clocks AHB1 Peripheral Clocks
 * @{
 */

 /**
 * @brief Enable AHB1 clock
 * @return None
 */
void enableRccAHB1Clk(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; // Enable GPIOD clock (bit 3)
}