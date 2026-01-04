#include "timer.h"

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

bool getUIF(void)
{
    return ((TIM2->SR & TIM_SR_UIF) != 0U);
}


void clearUIF(void)
{
    TIM2->SR &= ~TIM_SR_UIF;
}