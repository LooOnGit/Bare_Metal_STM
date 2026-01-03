#ifndef __SYS_STICK_H__
#define __SYS_STICK_H__

#define STM32F411xE
#include "stm32f4xx.h" 

#define CTRL_ENABLE    (1U << 0)
#define CTRL_CLCKSRC   (1U << 2)
#define CTRL_COUNTFLAG (1U << 16)

#define ONE_MSEC_TICKS 16000

void systickMsecDelay(uint32_t delay);

#endif // __SYS_STICK_H__