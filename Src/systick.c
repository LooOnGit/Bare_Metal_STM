/**
 * @file    systick.c
 * @brief   SysTick timer driver implementation for STM32F411
 * @author  Loo
 * @version 1.0
 * @date    2026-01-08
 *
 * Implements millisecond delay functionality using ARM Cortex-M4 SysTick timer.
 */

#include "systick.h"

/**
 * @brief Generate blocking delay in milliseconds using SysTick timer
 *
 * Uses the ARM Cortex-M4 SysTick timer to create precise millisecond delays
 * through polling the COUNTFLAG bit. This is a busy-wait implementation.
 *
 * Configuration:
 * - System clock: 16 MHz
 * - SysTick LOAD: ONE_MSEC_TICKS - 1 = 15999
 * - Result: 1 millisecond per COUNTFLAG toggle
 * - Total delay: delay × 1 ms
 *
 * @param delay Delay time in milliseconds
 *
 * @return None
 *
 * @note This is a blocking (busy-wait) function.
 *       During delay, processor cannot execute other code.
 *       Uses polling, not interrupts (TICKINT = 0).
 * 
 * @par Example:
 * @code
 * systickMsecDelay(500);  // Wait 500 milliseconds
 * @endcode
 *
 * @see SYSTICK module documentation
 */
void systickMsecDelay(uint32_t delay)
{
    /* Load number of clock cycles per millisecond*/
    SysTick->LOAD = ONE_MSEC_TICKS - 1;
    /*Clear systick current value register*/
    SysTick->VAL = 0;
    /*Select internal clock source*/
    SysTick->CTRL |= CTRL_CLCKSRC;
    /*Enable systick*/
    SysTick->CTRL |= CTRL_ENABLE;
    for (uint32_t i = 0; i < delay; i++)
    {
        /*Wait until the COUNTFLAG is set*/
        while ((SysTick->CTRL & CTRL_COUNTFLAG) == 0)
        {
            /*Do nothing*/
        }
    }
    /*Disable systick*/
    SysTick->CTRL &= ~CTRL_ENABLE;
}