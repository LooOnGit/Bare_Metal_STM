/**
 * @file    systick.h
 * @brief   SysTick timer driver for STM32F411
 * @author  Loo
 * @version 1.0
 * @date    2026-01-08
 *
 * This driver provides millisecond delay functionality using ARM Cortex-M4
 * SysTick timer. Implements blocking delays via polling.
 */

#ifndef __SYS_STICK_H__
#define __SYS_STICK_H__

#define STM32F411xE
#include "stm32f4xx.h" 

/**
 * @defgroup SYSTICK SysTick Timer Driver
 * @brief SysTick millisecond delay timer
 * @{
 */

/** SysTick CTRL register enable bit */
#define CTRL_ENABLE    (1U << 0)
/** SysTick CTRL register clock source bit (1 = processor clock) */
#define CTRL_CLCKSRC   (1U << 2)
/** SysTick CTRL register count flag bit */
#define CTRL_COUNTFLAG (1U << 16)

/** System clock ticks per millisecond (16 MHz / 1000) */
#define ONE_MSEC_TICKS 16000

/**
 * @brief Generate blocking delay in milliseconds
 * 
 * Uses SysTick timer to create precise millisecond delays.
 * Blocks until delay expires.
 * 
 * @param delay Delay time in milliseconds
 * 
 * @return None
 * 
 * @note This is a blocking (busy-wait) function.
 *       Uses polling, not interrupts (TICKINT = 0).
 * 
 * @par Example:
 * @code
 * systickMsecDelay(500);  // Wait 500 milliseconds
 * @endcode
 */
void systickMsecDelay(uint32_t delay);

/** @} */"

#endif // __SYS_STICK_H__