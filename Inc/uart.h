/**
 * @file    uart.h
 * @brief   UART low-level driver for STM32F411
 * @author  Loo
 * @version 1.0
 * @date    2026-01-08
 *
 * This driver provides UART2 communication on STM32F411 using PA2 (TX) pin.
 * Supports standard serial communication at 115200 baud rate.
 */

#ifndef __UART_H__
#define __UART_H__

#define STM32F411xE
#include "stm32f4xx.h" 
#include <stdint.h>

/** @defgroup UART UART Driver
 * @brief UART2 serial communication driver
 * @{
 */

/** System frequency in Hz */
#define SYS_FREQ 16000000U
/** UART debug baud rate */
#define DBG_UART_BAUDRATE 115200U
/** APB1 clock frequency (same as system clock for this MCU) */
#define APB1_CLK SYS_FREQ

/**
 * @brief Initialize UART2 peripheral
 * 
 * Configures GPIO PA2 as UART2 TX output with AF7 function.
 * Sets up UART2 with 115200 baud rate and enables transmitter.
 * 
 * @return None
 * 
 * @note Uses polling-based transmission, no interrupts enabled
 */
void uartInit(void);

/**
 * @brief Send a single character via UART
 * 
 * Waits for transmit register to be empty and sends one character.
 * This is a blocking operation.
 * 
 * @param c Character to send
 * 
 * @return None
 */
void uartSendChar(char c);

/**
 * @brief Send a null-terminated string via UART
 * 
 * Transmits each character in the string one by one until null terminator.
 * This is a blocking operation.
 * 
 * @param s Pointer to null-terminated string
 * 
 * @return None
 */
void uartSendString(const char *s);

/**
 * @brief Redirect putchar to UART output (standard I/O support)
 * 
 * Used by printf() to output characters to UART.
 * This is a required function for standard library I/O redirection.
 * 
 * @param ch Character code to send
 * 
 * @return Character code sent
 */
int __io_putchar(int ch);

/** @} */

#endif // __UART_H__