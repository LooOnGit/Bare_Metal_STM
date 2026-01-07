#ifndef __UART_H__
#define __UART_H__

#define STM32F411xE
#include "stm32f4xx.h" 
#include <stdint.h>

void uartInit(void);
static void uartWrite(int ch);
int __io_putchar(int ch);

static uint16_t computeUARTDB(uint32_t periph_clk, uint32_t baudRate);
static void setUartBaudrate(uint32_t periphClk, uint32_t baudRate);

#endif // __UART_H__