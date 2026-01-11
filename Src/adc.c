/**
 * @file    adc.c
 * @brief   ADC driver implementation for STM32F411
 * @author  Loo
 * @version 1.0
 * @date    2026-01-11
 *
 * Implements ADC1 configuration and polling-based analog conversion.
 */

#include "adc.h"

/**
 * @brief Initialize ADC1 peripheral with PA1 input
 * 
 * Configuration steps:
 * 1. Enable GPIOA clock (AHB1)
 * 2. Set PA1 mode to analog (MODER[2:1] = 11)
 * 3. Enable ADC1 clock (APB2)
 * 4. Configure conversion sequence: SQ1 = Channel 1 (PA1)
 * 5. Set sequence length to 1 channel
 * 6. Enable ADC module (ADON = 1)
 * 
 * @return None
 * 
 * @note PA1 pin must be connected to analog signal source
 * @note ADC ready but not converting until startConversion() called
 * @see startConversion()
 */
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

/**
 * @brief Start ADC continuous conversion
 * 
 * Enables continuous conversion mode (CR2.CONT = 1) and initiates
 * conversion via software trigger (CR2.SWSTART = 1).
 * 
 * Once started, ADC continuously converts PA1 analog input and
 * updates the data register. Read results with adcRead().
 * 
 * @return None
 * 
 * @note Must call pa1ADCInit() first to configure ADC
 * @note Conversion runs continuously until stopped
 * @see pa1ADCInit(), adcRead()
 */
void startConversion(void)
{
    /*Enable continuous conversion*/
    ADC1->CR2 |= ADC_CR2_CONT;

    /*Start ADC conversion*/
    ADC1->CR2 |= ADC_CR2_SWSTART;
}

/**
 * @brief Read ADC conversion result (blocking)
 * 
 * Polls the EOC (End of Conversion) flag until conversion completes,
 * then reads and returns the 12-bit result from the data register.
 * 
 * @return 12-bit ADC value (0-4095 for 12-bit resolution)
 *         Corresponding to 0V - VREF (typically 0-3.3V)
 * 
 * @return None
 * 
 * @note This is a blocking call - waits for EOC flag
 * @note Only call after startConversion() has been invoked
 * @note For continuous conversion, each call gets the latest result
 * 
 * @par Conversion formula:
 * @code
 * voltage = (adcRead() / 4095.0) * 3.3;  // Assuming VREF = 3.3V
 * @endcode
 * 
 * @see startConversion()
 */
uint32_t adcRead(void)
{
    /*Wait for conversion to complete*/
    while(!(ADC1->SR & ADC_SR_EOC)){}

    /*Read conversion value*/
    return ADC1->DR;
}