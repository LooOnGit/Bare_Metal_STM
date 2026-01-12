/**
 * @file spi.c
 * @brief SPI (Serial Peripheral Interface) driver implementation for STM32F411
 * 
 * This file contains the implementation of SPI1 initialization, configuration,
 * and data transmission/reception functions for the STM32F411 microcontroller.
 * 
 * @author Bare Metal STM32
 * @version 1.0
 * @date 2026
 */

#include "spi.h"

/**
 * @brief Initialize SPI1 GPIO pins
 * 
 * @details
 * Enables clock access to GPIOA and configures the following pins:
 * - PA5: Alternate Function (SCK - Serial Clock)
 * - PA6: Alternate Function (MISO - Master In Slave Out)
 * - PA7: Alternate Function (MOSI - Master Out Slave In)
 * - PA9: General Purpose Output (CS - Chip Select)
 * 
 * All pins are configured with their respective alternate function types
 * for SPI1 operation.
 * 
 * @return void
 * @see spi1Config()
 */
void spiInit(void)
{
    /*Enable clock access to GPIOA*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /*Set PA5, PA6, PA7 mode to alternate function*/
    
    /*PA5*/
    GPIOA->MODER &= ~(1U<<10);
    GPIOA->MODER |=  (1U<<11);

    /*PA6*/
    GPIOA->MODER &= ~(1U<<12);
    GPIOA->MODER |=  (1U<<13);
    
    /*PA7*/
    GPIOA->MODER &= ~(1U<<14);
    GPIOA->MODER |=  (1U<<15);

    /*PA9*/
    GPIOA->MODER |= (1U<<18);
    GPIOA->MODER &= ~(1U<<19);

    /*Set PA5, PA6, PA7 alternate function type to SPI1*/
    /*PA5*/
    GPIOA->AFR[0] |= (1U<<20);
    GPIOA->AFR[0] &= ~(1U<<21);
    GPIOA->AFR[0] |= (1U<<22);
    GPIOA->AFR[0] &= ~(1U<<23);

    /*PA6*/
    GPIOA->AFR[0] &= ~(1U<<24);
    GPIOA->AFR[1] |= (1U<<25);
    GPIOA->AFR[2] |= (1U<<26);
    GPIOA->AFR[3] &= ~(1U<<27);

    /*PA7*/
    GPIOA->AFR[0] |= (1U<<28);
    GPIOA->AFR[1] |= (1U<<29);
    GPIOA->AFR[2] |= (1U<<30);
    GPIOA->AFR[3] &= ~(1U<<31);
}

/**
 * @brief Configure and enable SPI1 peripheral
 * 
 * @details
 * Performs the following configuration:
 * - Enables the SPI1 clock via RCC APB2
 * - Sets the clock prescaler to fPCLK/4
 * - Configures clock polarity (CPOL = 1) - CK remains 1 when idle
 * - Configures clock phase (CPHA = 1) - Data captured on second edge
 * - Enables full-duplex mode
 * - Sets MSB first bit order
 * - Configures as Master mode
 * - Sets 8-bit data frame format
 * - Enables software slave management (SSM = 1, SSI = 1)
 * - Enables the SPI module
 * 
 * @note spiInit() must be called before this function to configure GPIO pins
 * @return void
 * @see spiInit()
 */
void spi1Config(void)
{
    /*Enable clock access to SPI1*/
    RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

    /*Set clock to fPCLK/4*/
    SPI1->CR1 |= (1U<<3);
    SPI1->CR1 &= ~(1U<<4);
    SPI1->CR1 &= ~(1U<<5);

    /*Set CPOL to 1 and CPHA to 1*/
    SPI1->CR1 |= (1U<<1);
    SPI1->CR1 |= (1U<<0);

    /*Enable full duplex*/
    SPI1->CR1 &= ~(1U<<10);

    /*Set MSB first*/
    SPI1->CR1 &= ~(1U<<7);

    /*Set mode to MASTER*/
    SPI1->CR1 |= (1U<<2);

    /*Set 8 bit data mode*/
    SPI1->CR1 &= ~(1U<<11);

    /*Select software slave management by setting SSM=1 and SSI=1*/
    SPI1->CR1 |= (1U<<9);
    SPI1->CR1 |= (1U<<8);

    /*Enable SPI module*/
    SPI1->CR1 |= (1U<<6);
}

/**
 * @brief Transmit data via SPI1
 * 
 * @details
 * Sends data through the SPI bus in blocking mode. The function:
 * 1. Waits for the Transmit Buffer Empty (TXE) flag for each byte
 * 2. Writes the byte to the SPI Data Register (DR)
 * 3. After all data is transmitted, waits for TXE to be set
 * 4. Waits for the Busy (BSY) flag to clear
 * 5. Clears any potential overflow flag by dummy reading DR and SR
 * 
 * @param[in] data Pointer to the buffer containing data to transmit
 * @param[in] size Number of bytes to transmit
 * 
 * @note - Blocking function; does not return until transmission is complete
 *       - Slave must be selected using csEnable() before calling this function
 *       - Function blocks until BSY flag is cleared
 * 
 * @return void
 * @see csEnable(), spi1Receive()
 */
void spi1Transmit(uint8_t *data, uint32_t size)
{
    uint32_t i = 0;
    uint8_t temp;
    while(i<size)
    {
        /*Wait until TXE is set*/
        while(!(SPI1->SR & (SPI_SR_TXE))){}

        /*Write the data to the data register*/
        SPI1->DR = data[i];
        i++;
    }
    
    /*Wait until TXE is set*/
    while(!(SPI1->SR & (SPI_SR_TXE))){}

    /*Wait for BUSY flag to reset*/
    while(SPI1->SR & (SPI_SR_BSY)){}

    /*Clear OVR flag*/
    temp = SPI1->DR;
    temp = SPI1->SR;
}

/**
 * @brief Receive data via SPI1
 * 
 * @details
 * Receives data from the SPI bus by transmitting dummy data (0x00) to generate
 * the required clock signals. The function:
 * 1. Writes 0x00 to the SPI Data Register to start SPI clock
 * 2. Waits for the Receive Buffer Not Empty (RXNE) flag
 * 3. Reads received data from the Data Register
 * 4. Repeats until all requested bytes are received
 * 
 * @param[out] data Pointer to the buffer where received data will be stored
 * @param[in] size Number of bytes to receive
 * 
 * @note - Blocking function; does not return until all data is received
 *       - Slave must be selected using csEnable() before calling this function
 *       - Dummy bytes (0x00) are transmitted to generate the clock
 * 
 * @return void
 * @see csEnable(), spi1Transmit()
 */
void spi1Receive(uint8_t *data, uint32_t size)
{
    while(size)
    {
        /*Send dummy data*/
        SPI1->DR = 0;
        /*Wait for RXNE flag to be set*/
        while(!(SPI1->SR & (SPI_SR_RXNE))){}
        /*Read data from data register*/
        *data++ = (SPI1->DR);
        size--;
    }
}

/**
 * @brief Enable chip select
 * 
 * @details
 * Pulls PA9 (Chip Select) to low logic level, selecting the slave device
 * for SPI communication. This function must be called before any SPI
 * transmission or reception operation.
 * 
 * @return void
 * @see csDisable(), spi1Transmit(), spi1Receive()
 */
void csEnable(void)
{
    GPIOA->ODR &=~ (1U<<9);
}

/**
 * @brief Disable chip select
 * 
 * @details
 * Pulls PA9 (Chip Select) to high logic level, deselecting the slave device.
 * This function must be called after SPI transmission or reception operations
 * are complete to signal the end of communication.
 * 
 * @return void
 * @see csEnable(), spi1Transmit(), spi1Receive()
 */
void csDisable(void)
{
    GPIOA->ODR |= (1U<<9);
}