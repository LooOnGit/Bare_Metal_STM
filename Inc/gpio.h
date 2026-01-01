#ifndef __GPIO__H__
#define __GPIO__H__
/**
 * @file    gpio.h
 * @brief   GPIO low-level driver for STM32F411
 * @author  Loo
 * @version 1.0
 * @date    2025-1-1
 *
 * This driver provides basic GPIO control functions such as
 * clock enable, pin initialization, read/write and toggle.
 * It is implemented using CMSIS registers (no HAL dependency).
 */
#define STM32F411xE
#include "stm32f4xx.h" 
#include "clock.h"

/**
 * @defgroup GPIO GPIO Driver
 * @brief Low-level GPIO driver
 * @{
 */


 #define ON     1


void GPIO_InitPin(void);

void GPIO_WritePin(GPIO_TypeDef *port,
                   uint32_t pin,
                   uint32_t state);

uint32_t GPIO_ReadPin(GPIO_TypeDef *port,
                      uint32_t pin);

void GPIO_TogglePin(GPIO_TypeDef *port,
                    uint32_t pin);

/** @} */ /* end of GPIO */


#endif // __GPIO__H__
