/**
 * @file spi.h
 * @brief SPI (Serial Peripheral Interface) driver header file for STM32F411
 * 
 * This module provides functions for configuring and using the SPI1 peripheral
 * on the STM32F411 microcontroller. It handles initialization of GPIO pins,
 * SPI configuration, data transmission/reception, and chip select control.
 * 
 * @details
 * The driver uses the following pins:
 * - PA5: SCK (Serial Clock)
 * - PA6: MISO (Master In Slave Out)
 * - PA7: MOSI (Master Out Slave In)
 * - PA9: CS (Chip Select)
 * 
 * The SPI is configured in Master mode with:
 * - Clock polarity (CPOL) = 1
 * - Clock phase (CPHA) = 1
 * - 8-bit data format
 * - MSB first
 * - Full-duplex mode
 * - Clock frequency = fPCLK/4
 * 
 * @author Bare Metal STM32
 * @version 1.0
 * @date 2026
 */

#ifndef __SPI_H__
#define __SPI_H__

#define STM32F411xE
#include "stm32f4xx.h"

/**
 * @brief Initialize SPI1 GPIO pins
 * 
 * Configures PA5, PA6, PA7, and PA9 as alternate functions for SPI1 operation.
 * - PA5: SCK (Serial Clock)
 * - PA6: MISO (Master In Slave Out)
 * - PA7: MOSI (Master Out Slave In)
 * - PA9: CS (Chip Select) - General Purpose Output
 * 
 * @return void
 */
void spiInit(void);

/**
 * @brief Configure and enable SPI1 peripheral
 * 
 * Sets up the SPI1 control registers with the following configuration:
 * - Master mode enabled
 * - Clock prescaler = fPCLK/4
 * - Clock polarity (CPOL) = 1 (CK to 1 when idle)
 * - Clock phase (CPHA) = 1 (Data captured on 2nd edge)
 * - Full-duplex mode
 * - 8-bit data frame format
 * - MSB transmitted first
 * - Software slave management enabled
 * - SPI module enabled
 * 
 * @note Call spiInit() before calling this function
 * @return void
 */
void spi1Config(void);

/**
 * @brief Transmit data via SPI1
 * 
 * Sends data over the SPI bus. This function blocks until all data is transmitted
 * and the SPI bus becomes idle. The slave must be selected before calling this function.
 * 
 * @param[in] data Pointer to the data buffer containing the bytes to transmit
 * @param[in] size Number of bytes to transmit
 * 
 * @note The function waits for TXE (Transmit buffer empty) flag before each transmission
 *       and waits for BSY (Busy) flag to clear before returning
 * @return void
 */
void spi1Transmit(uint8_t *data, uint32_t size);

/**
 * @brief Receive data via SPI1
 * 
 * Receives data from the SPI bus by transmitting dummy data (0x00) to generate
 * the SPI clock. This function blocks until all requested data is received.
 * 
 * @param[out] data Pointer to the buffer where received data will be stored
 * @param[in] size Number of bytes to receive
 * 
 * @note The slave must be selected before calling this function
 *       Dummy data (0x00) is transmitted to generate the clock
 * @return void
 */
void spi1Receive(uint8_t *data, uint32_t size);

/**
 * @brief Enable chip select (pull CS low)
 * 
 * Sets PA9 to low logic level, selecting the slave device.
 * This function must be called before any SPI transmission or reception.
 * 
 * @return void
 */
void csEnable(void);

/**
 * @brief Disable chip select (pull CS high)
 * 
 * Sets PA9 to high logic level, deselecting the slave device.
 * This function must be called after SPI transmission or reception is complete.
 * 
 * @return void
 */
void csDisable(void);


#endif // __SPI_H__