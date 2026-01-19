#include "exti.h"
#include "gpio.h"

/**
 * @brief Initialize EXTI0 on PA0 pin for falling edge interrupt
 * 
 * Configuration:
 * - PA0 as input pin
 * - Falling edge trigger
 * - EXTI0 interrupt enabled in NVIC
 */
void pc13ExtiInit(void)
{
    /*Disable global interrupts*/
    __disable_irq();

    /*Enable clock access for GPIOA*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /*Set PA0 as input (clear MODER bits)*/
    GPIOA->MODER &= ~(GPIO_MODER_MODER0_0 | GPIO_MODER_MODER0_1);
    
    /*Enable clock access to SYSCFG*/
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

    /*Select PORTA for EXTI0 (use EXTICR[0] for EXTI0)*/
    SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;

    /*Unmask EXTI0 interrupt*/
    EXTI->IMR |= EXTI_IMR_MR0;

    /*Select falling edge trigger for EXTI0*/
    EXTI->FTSR |= EXTI_FTSR_TR0;

    /*Enable EXTI0 line in NVIC*/
    NVIC_EnableIRQ(EXTI0_IRQn);

    /*Enable global interrupt*/
    __enable_irq();
}
