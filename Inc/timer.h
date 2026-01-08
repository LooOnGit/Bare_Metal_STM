/**
 * @file    timer.h
 * @brief   Timer/TIM2 driver for STM32F411
 * @author  Loo
 * @version 1.0
 * @date    2026-01-08
 *
 * This driver provides TIM2 (32-bit General Purpose Timer) functions.
 * Configured for 1 Hz update event using polling.
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#define STM32F411xE
#include "stm32f4xx.h" 
#include <stdbool.h>

/**
 * @defgroup TIMER Timer Driver
 * @brief TIM2 general purpose timer control
 * @{
 */

/**
 * @brief Initialize TIM2 timer
 * 
 * Configures TIM2 with 1 Hz update rate.
 * 
 * @return None
 * @note Uses polling - no interrupts enabled
 * @see getUIF(), clearUIF()
 */
void timInit(void);

/**
 * @brief Check TIM2 update interrupt flag (UIF)
 * 
 * Reads the update interrupt flag without clearing it.
 * 
 * @return true if update event occurred, false otherwise
 * @note Does NOT clear the flag. Use clearUIF() to clear.
 * @see clearUIF()
 */
bool getUIF(void);

/**
 * @brief Clear TIM2 update interrupt flag (UIF)
 * 
 * Clears the flag after detecting an event.
 * 
 * @return None
 * @see getUIF()
 */
void clearUIF(void);

/** @} */"