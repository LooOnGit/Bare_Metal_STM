#ifndef __SPI_H__
#define __SPI_H__

#define STM32F411xE
#include "stm32f4xx.h"

void spiInit(void);
void spi1Config(void);
void spi1Transmit(uint8_t *data, uint32_t size);
void spi1Receive(uint8_t *data, uint32_t size);
void csEnable(void);
void csDisable(void);


#endif // __SPI_H__