#ifndef __EXTI__H__
#define __EXTI__H__

#define STM32F411xE
#include "stm32f4xx.h" 

/**
 * @brief Initialize EXTI0 on PA0 pin for falling edge interrupt
 */
void pc13ExtiInit(void);

/**
 * @brief EXTI0 interrupt handler (PA0)
 */
void EXTI0_IRQHandler(void);

#endif //__EXTI__H__