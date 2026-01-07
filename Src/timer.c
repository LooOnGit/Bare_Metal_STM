#include "timer.h"

/**
 * @brief  Initialize TIM2 as a basic time base.
 *
 * This function configures TIM2 to generate an update event
 * at a fixed period using polling (UIF flag).
 *
 * Configuration details:
 * - Clock source   : APB1 (TIM2)
 * - Prescaler      : 1600 - 1  → Timer clock = 1 kHz (from 16 MHz)
 * - Auto-reload    : 10000 - 1 → Update event every 1 second
 * - Counter mode   : Up-counting
 *
 * After initialization, the timer starts immediately.
 *
 * @note This timer does not use interrupts. The update event
 *       must be checked by polling the UIF flag.
 */
void timInit(void)
{
    /*Enable clock access to tim2*/
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    /*Set prescaler value*/
    TIM2->PSC = 1600 - 1;  // 16 MHz / 16000 = 1 KHz
    /*Set auto-reload value*/
    TIM2->ARR = 10000 - 1;   // 1 KHz / 1000 = 1 Hz
    /*Clear counter*/
    TIM2->CNT = 0;
    /*Enable timer*/
    TIM2->CR1 |= TIM_CR1_CEN;
}

/**
 * @brief  Check TIM2 update interrupt flag (UIF).
 *
 * This function checks whether the timer update event has occurred.
 *
 * @retval true   Update event occurred (UIF = 1)
 * @retval false  No update event (UIF = 0)
 *
 * @note This function does NOT clear the UIF flag.
 */
bool getUIF(void)
{
    return ((TIM2->SR & TIM_SR_UIF) != 0U);
}

/**
 * @brief  Clear TIM2 update interrupt flag (UIF).
 *
 * This function clears the UIF flag in the TIM2 status register.
 * It should be called after detecting an update event.
 *
 * @note Writing 0 clears the UIF bit.
 */
void clearUIF(void)
{
    TIM2->SR &= ~TIM_SR_UIF;
}