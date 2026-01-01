#ifndef __CLOCK_H__
#define __CLOCK_H__
/**
 * @file    rcc.h
 * @brief   RCC driver for STM32F411 - Peripheral Clock Control
 * @author  Your Name
 * @version 1.0
 * @date    2024-12-31
 *
 * This driver provides functions to enable/disable peripheral clocks.
 */

#define STM32F411xE
#include "stm32f4xx.h" 


void RCC_AHB1_CLK_ENABLE(void);




#endif // __CLOCK_H__