#ifndef __GPIO__H__
#define __GPIO__H__

#define STM32F411xE
#include "stm32f4xx.h" 



#define GPIOD_EN (1U<<3) //Bit mask for enabling GPIOD (bit 0)
#define LED_PIN (1U<<12) //Bit 

void initLed(void);
void ledOn(void);
void ledOff(void);

#endif // __GPIO__H__
