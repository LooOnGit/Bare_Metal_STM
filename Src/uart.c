/**
 * @file    uart.c
 * @brief   UART low-level driver implementation for STM32F411
 * @author  Loo
 * @version 1.0
 * @date    2026-01-08
 *
 * Implements UART2 communication driver using polling-based transmission.
 * Supports standard serial output via PA2 (TX) pin.
 */

#include "uart.h"
#include <stdint.h>

/**
 * @brief Initialize UART2 peripheral with GPIO and clock configuration
 * 
 * Performs the following initialization steps:
 * - Enables GPIOA clock
 * - Configures PA2 as alternate function (AF7 = UART2_TX)
 * - Enables UART2 peripheral clock on APB1
 * - Sets baud rate to 115200
 * - Enables UART transmitter
 * - Enables UART module
 * 
 * @return None
 * 
 * @note PA2 must be connected to the UART interface
 */
void uartInit(void)
{
    /*Enable clock access to GPIOA*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /*Set the mode of PA2 to alternate function mode*/
    GPIOA->MODER &= ~(GPIO_MODER_MODE2_0);
    GPIOA->MODER |= (GPIO_MODER_MODE2_1);

    /*Set alternate function type to AF7 (UART2_TX)*/
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_0;
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_1;
    GPIOA->AFR[0] |= GPIO_AFRL_AFSEL2_2;
    GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL2_3;

    /*Enable clock access to UART2*/
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;

    /*Configure uart baudrate*/
    setUartBaudrate(APB1_CLK, DBG_UART_BAUDRATE);

    /*Configure transfer direction*/
    USART2->CR1 |= USART_CR1_TE;

    /*Enable UART Module*/
    USART2->CR1 |= USART_CR1_UE;

}

/**
 * @brief Low-level UART write function
 * 
 * Waits for the transmit data register to be empty, then writes
 * one character to the UART data register.
 * 
 * @param ch Character to transmit (only lower 8 bits used)
 * 
 * @return None
 * 
 * @note This is a blocking call - waits until transmit register is ready
 */
static void uartWrite(int ch)
{
    /*Make sure transmit data register is empty*/
    while(!(USART2->SR & USART_SR_TXE)){}

    /*Write to transmit data register*/
    USART2->DR = (ch & 0xFF);
}

/**
 * @brief Compute UART baud rate divisor
 * 
 * Calculates the BRR (Baud Rate Register) value using the formula:
 * BRR = (periphClk + baudRate/2) / baudRate
 * 
 * The addition of baudRate/2 implements round-half-up rounding.
 * 
 * @param periphClk Peripheral clock frequency in Hz
 * @param baudRate  Desired baud rate in bits per second
 * 
 * @return 16-bit BRR value for UART configuration
 * 
 * @note Typical values: 16MHz clock with 115200 baud = 139
 */
static uint16_t computeUARTDB(uint32_t periphClk, uint32_t baudRate)
{
    return (uint16_t)((periphClk + baudRate/2) / baudRate); //round-half-up
}

/**
 * @brief Configure UART2 baud rate
 * 
 * Writes the calculated baud rate divisor to the UART2 Baud Rate Register.
 * 
 * @param periphClk Peripheral clock frequency in Hz
 * @param baudRate  Desired baud rate in bits per second
 * 
 * @return None
 * 
 * @see computeUARTDB()
 */
static void setUartBaudrate(uint32_t periphClk, uint32_t baudRate)
{
    USART2->BRR = computeUARTDB(periphClk, baudRate);
}

/**
 * @brief Standard library putchar redirect
 * 
 * Required for printf() support. Sends character to UART via uartWrite().
 * This allows standard C library functions to output to UART.
 * 
 * @param ch Character to output
 * 
 * @return Character code sent
 * 
 * @note This is called automatically by printf(), puts(), etc.
 */
int __io_putchar(int ch)
{
    uartWrite(ch);
    return ch;
}

/**
 * @brief Send a single character via UART2
 * 
 * Waits for transmit data register to be empty, then sends one character.
 * Blocking operation.
 * 
 * @param c Character to send
 * 
 * @return None
 * 
 * @note Waits for TXE (Transmit Data Register Empty) flag
 */
void uartSendChar(char c)
{
    while (!(USART2->SR & USART_SR_TXE));
    USART2->DR = c;
}

/**
 * @brief Send null-terminated string via UART2
 * 
 * Transmits each character from the string one by one until 
 * null terminator is encountered. Blocking operation.
 * 
 * @param s Pointer to null-terminated character string
 * 
 * @return None
 * 
 * @note String must be null-terminated
 * @see uartSendChar()
 * 
 * @par Example:
 * @code
 * uartSendString("Hello, STM32!\r\n");
 * @endcode
 */
void uartSendString(const char *s)
{
    while (*s)
    {
        uartSendChar(*s++);
    }
}