/**
 * @file    adc.h
 * @brief   ADC (Analog-to-Digital Converter) driver for STM32F411
 * @author  Loo
 * @version 1.0
 * @date    2026-01-11
 *
 * This driver provides ADC1 (12-bit converter) functionality for
 * analog input measurement via PA1. Supports single and continuous
 * conversion modes using polling.
 */

#ifndef __ADC_H__
#define __ADC_H__

#define STM32F411xE
#include "stm32f4xx.h"

/**
 * @defgroup ADC ADC Driver
 * @brief ADC1 12-bit analog-to-digital converter control
 * @{
 */

/** ADC conversion sequence length = 1 channel */
#define ADC_SEQ_LEN_1      (0x00)

/**
 * @brief Initialize ADC1 with PA1 input
 * 
 * Configures:
 * - PA1 as analog input (MODER = 11)
 * - ADC1 clock (APB2)
 * - Conversion sequence: Channel 1 (PA1)
 * - ADC module enabled
 * 
 * @return None
 * 
 * @note ADC is not in conversion mode until startConversion() is called
 * @see startConversion(), adcRead()
 */
void pa1ADCInit(void);

/**
 * @brief Start ADC continuous conversion
 * 
 * Enables continuous conversion mode and starts the ADC conversion
 * via software trigger (SWSTART).
 * 
 * @return None
 * 
 * @note Must call pa1ADCInit() first
 * @note After calling, conversion runs continuously
 * @see pa1ADCInit(), adcRead()
 */
void startConversion(void);

/**
 * @brief Read ADC conversion result
 * 
 * Waits for conversion to complete (EOC flag), then reads
 * the 12-bit result from ADC data register.
 * 
 * @return 12-bit ADC conversion value (0-4095)
 * 
 * @note Blocking call - waits for EOC flag
 * @note Call after startConversion() begins
 * @see startConversion()
 */
uint32_t adcRead(void);
#endif // __ADC_H__