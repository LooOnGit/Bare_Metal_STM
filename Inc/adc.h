#ifndef __ADC_H__
#define __ADC_H__

#define STM32F411xE
#include "stm32f4xx.h" 


#define ADC_SEQ_LEN_1      (0x00)

void pa1ADCInit(void);
void startConversion(void);
uint32_t adcRead(void);
#endif // __ADC_H__