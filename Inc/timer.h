#ifndef __TIMER_H__
#define __TIMER_H__

#define STM32F411xE
#include "stm32f4xx.h" 
#include <stdbool.h>

void timInit(void);
bool getUIF(void);
void clearUIF(void);

#endif  // __TIMER_H__