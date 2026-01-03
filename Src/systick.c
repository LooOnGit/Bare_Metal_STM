#include "systick.h"

/**
 * @brief Generate blocking delay in milliseconds using SysTick timer
 *
 * @param delay Delay time in milliseconds
 *
 * @note This function uses polling (busy-wait) and does not use
 *       SysTick interrupt (TICKINT = 0).
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