#include "adc.h"

void pa1ADCInit(void)
{
    /*Configure the ADC GPIO Pin*/
    /*Enable clock access to GPIOA*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /*Set PA1 mode to analog mode*/
    GPIOA->MODER |= GPIO_MODER_MODER1;

    /*Configure the ADC module*/
    /*Enable clock access to the ADC module*/
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    /*Set conversion sequence start*/
    ADC1->SQR3 = ADC_SQR3_SQ1;

    /*Set conversion sequence lenght*/
    ADC1->SQR1 = ADC_SEQ_LEN_1;

    /*Enable ADC module*/
    ADC1->CR2 |= ADC_CR2_ADON;
}

void startConversion(void)
{
    /*Enable continuous conversion*/
    ADC1->CR2 |= ADC_CR2_CONT;

    /*Start ADC conversion*/
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

uint32_t adcRead(void)
{
    /*Wait for conversion to complete*/
    while(!(ADC1-SR & ADC_SR_EOC)){}

    /*Read conversion value*/
    return ADC1->DR;
}