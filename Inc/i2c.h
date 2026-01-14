/**
 * @file i2c.h
 * @brief I2C (Inter-Integrated Circuit) driver for STM32F411xE.
 * 
 * This header file provides declarations for I2C1 interface functions
 * used for communication with I2C slave devices. The module is configured
 * for standard mode (100 kHz) on GPIO pins PB8 (SCL) and PB9 (SDA).
 * 
 * @author Bare Metal STM32
 * @date 2026
 */

#ifndef __I2C_H__
#define __I2C_H__

#define STM32F411xE
#include "stm32f4xx.h" 

/**
 * @brief Initialize I2C1 peripheral.
 * 
 * Configures I2C1 for standard mode (100 kHz) communication. Sets up:
 * - GPIO pins PB8 (SCL) and PB9 (SDA) as alternate function (AF4)
 * - Open-drain output with pull-up resistors
 * - I2C clock to 100 kHz
 * - Rise time for standard mode
 * 
 * @return void
 * 
 * @note Must be called before any other I2C operations.
 * @note Requires prior GPIO and clock initialization.
 */
void i2c1Init(void);

/**
 * @brief Read a single byte from I2C slave device.
 * 
 * Reads one byte from a specific memory address on the I2C slave device
 * using a write-restart-read sequence.
 * 
 * @param[in] saddr Slave device address (7-bit address, will be shifted left by 1)
 * @param[in] msddr Memory address to read from on the slave device
 * @param[out] data Pointer to buffer where the read byte will be stored
 * 
 * @return void
 * 
 * @note This function blocks until the byte is received.
 * @note The function automatically sends STOP condition after reading.
 * @warning Ensure EEPROM has sufficient time to respond between write and read cycles.
 */
void i2c1ByteRead(uint8_t saddr, char msddr, char* data);

/**
 * @brief Read multiple bytes from I2C slave device in burst mode.
 * 
 * Reads n consecutive bytes from a starting memory address on the I2C slave
 * device using a single I2C transaction with write-restart-read sequence.
 * 
 * @param[in] saddr Slave device address (7-bit address, will be shifted left by 1)
 * @param[in] maddr Starting memory address to read from on the slave device
 * @param[in] n Number of bytes to read (must be > 0)
 * @param[out] data Pointer to buffer where read bytes will be stored
 * 
 * @return void
 * 
 * @note This function blocks until all bytes are received.
 * @note The function automatically sends STOP condition after the last byte.
 * @note Buffer pointed by 'data' must be large enough to hold 'n' bytes.
 * @warning Automatic ACK/NACK is handled internally for burst reads.
 */
void i2c1BurstRead(char saddr, char maddr, int n, char* data);

/**
 * @brief Write multiple bytes to I2C slave device in burst mode.
 * 
 * Writes n consecutive bytes to a starting memory address on the I2C slave
 * device using a single I2C transaction.
 * 
 * @param[in] saddr Slave device address (7-bit address, will be shifted left by 1)
 * @param[in] maddr Starting memory address to write to on the slave device
 * @param[in] n Number of bytes to write (must be > 0)
 * @param[in] data Pointer to buffer containing bytes to write
 * 
 * @return void
 * 
 * @note This function blocks until all bytes are transmitted.
 * @note The function automatically sends STOP condition after all data is written.
 * @note Buffer pointed by 'data' must contain at least 'n' bytes.
 * @warning Ensure slave device is ready to accept writes (write enable, etc.).
 */
void i2c1BurstWrite(char saddr, char maddr, int n, char* data);

#endif // __I2C_H__
