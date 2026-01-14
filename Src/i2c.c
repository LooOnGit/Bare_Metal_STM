/**
 * @file i2c.c
 * @brief I2C (Inter-Integrated Circuit) driver implementation for STM32F411xE.
 * 
 * Implementation of I2C1 peripheral driver providing byte and burst
 * read/write operations for I2C slave devices.
 * 
 * @author Bare Metal STM32
 * @date 2026
 */

#include "i2c.h"

/**
 * @brief Initialize I2C1 peripheral.
 * 
 * Configuration steps:
 * 1. Enable GPIO Bank B clock
 * 2. Configure PB8 and PB9 as alternate function (AF4 = I2C1)
 * 3. Set pins to open-drain output with pull-up resistors
 * 4. Enable I2C1 peripheral clock
 * 5. Reset I2C1 module
 * 6. Configure I2C clock frequency to 100 kHz
 * 7. Set rise time for standard mode
 * 8. Enable I2C1 peripheral
 * 
 * @details
 * - GPIO Configuration: PB8 (SCL) and PB9 (SDA)
 * - I2C Clock: 100 kHz (standard mode)
 * - CCR value: 80 (calculated from FPCLK1=16MHz, FSCL=100kHz)
 * - Rise time: 17 (for standard mode)
 * - Output type: Open-drain with pull-ups (required for I2C)
 * 
 * @return void
 * @see i2c1ByteRead, i2c1BurstRead, i2c1BurstWrite
 */
void i2c1Init(void)
{
    /*Enable clock access t GPIOB*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

    /*Set PB8 and Pb9 mode to alternate function*/
    GPIOB->MODER &= ~(GPIO_MODER_MODER8_0);
    GPIOB->MODER |= GPIO_MODER_MODER8_1;

    GPIOB->MODER &= ~(GPIO_MODER_MODER9_0);
    GPIOB->MODER |= GPIO_MODER_MODER9_1;

    /*Set Pb8 and Pb9 output type to open drain*/
    GPIOB->OTYPER |= (GPIO_OTYPER_OT8);
    GPIOB->OTYPER |= (GPIO_OTYPER_OT9);

    /*Enable pull-up for PB8 and PB9*/
    GPIOB->PUPDR |= (GPIO_PUPDR_PUPDR8_0);
    GPIOB->PUPDR |= (GPIO_PUPDR_PUPDR8_1);

    GPIOB->PUPDR |= (GPIO_PUPDR_PUPDR9_0);
    GPIOB->PUPDR |= (GPIO_PUPDR_PUPDR9_1);

    /*Set PB8 and PB9 alternate function type to I2C (AF4)*/
    GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL8_0);
    GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL8_1);
    GPIOB->AFR[1] |= ~(GPIO_AFRH_AFSEL8_2);
    GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL8_3);

    GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL9_0);
    GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL9_1);
    GPIOB->AFR[1] |= ~(GPIO_AFRH_AFSEL9_2);
    GPIOB->AFR[1] &= ~(GPIO_AFRH_AFSEL9_3);

    /*Enable clock access to I2C*/
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    /*Enable reset mode*/
    I2C1->CR1 |= I2C_CR1_SWRST;

    /*Come out of reset mode*/
    I2C1->CR1 &= ~(I2C_CR1_SWRST);

    /*Set Peripheral clock frequency*/
    I2C1->CR2 |= (1U<<4); //16MHz

    /*Set I2C to standard mode, 100kHz clock*/
    I2C1->CCR |= (1U<<12); // CCR=(FPCLK1)/​​(2×FSCL) = (16000000)/(2×100000)​ ​= 80​

    /*Set rise time*/
    I2C1->TRISE = 17; //T(RISE)=F(PCLK1)​×t(rise)​+1 = 16MHzx(1000ns)+1=17​

    /*Enable I2C1 module*/
    I2C1->CR1 |= I2C_CR1_PE;
}


/**
 * @brief Read a single byte from I2C slave device.
 * 
 * Performs a write-restart-read I2C transaction to read one byte from
 * a specific memory address on the slave device. The sequence:
 * 1. Waits for I2C bus to be free
 * 2. Sends START condition and slave address with write bit
 * 3. Sends memory address
 * 4. Sends RESTART condition
 * 5. Sends slave address with read bit
 * 6. Disables ACK to indicate last byte
 * 7. Receives byte and sends STOP condition
 * 
 * @param[in] saddr 7-bit slave device address (will be shifted left by 1 for R/W bit)
 * @param[in] msddr Memory address to read from (1 byte address)
 * @param[out] data Pointer to uint8_t variable to store received byte
 * 
 * @return void
 * 
 * @note This function is blocking and does not return until byte is received
 * @note Suitable for reading from EEPROM and similar devices with 1-byte addressing
 * @warning Caller must ensure data pointer is valid and points to writable memory
 * @see i2c1BurstRead for reading multiple consecutive bytes
 */
void i2c1ByteRead(uint8_t saddr, char msddr, char* data)
{
    volatile int tmp;

    /*Wait until bus not busy*/
    while(!(I2C1->SR2 & I2C_SR2_BUSY)){}

    /*Generate start condition*/
    I2C1->CR1 |= I2C_SR1_SB;

    /*Wait until start flag is set*/
    while(!(I2C1->SR1 & I2C_SR1_SB)){}

    /*Transmit slave address + Write*/
    I2C1->DR = (saddr << 1); //Write operation

    /*Wait until addr flag is set*/
    while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

    /*Clear addr flag*/
    tmp = I2C1->SR2;

    /*Send memory address*/
    I2C1->DR = msddr;

    /*Wait until transmit empty*/
    while(!(I2C1->SR1 & I2C_SR1_TXE)){}

    /*Generate restart*/
    I2C1->CR1 |= I2C_CR1_START;

    /*Wait until start flag is set*/
    while(!(I2C1->SR1 & I2C_SR1_SB)){}

    /*Transmit slave address + Read*/
    I2C1->DR = (saddr << 1) | 1; //Read operation

    /*Wait until addr flag is set*/
    while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

    /*Disable Acknowledge*/
    I2C1->CR1 &= ~I2C_CR1_ACK;

    /*Clear addr flag*/
    tmp = I2C1->SR2;

    /*Generate stop after data received*/
    I2C1->CR1 |= I2C_CR1_STOP;

    /*Wait until RXNE flag is set*/
    while(!(I2C1->SR1 & I2C_SR1_RXNE)){}

    /*Read data from DR*/
    *data++ = I2C1->DR;
}


/**
 * @brief Read multiple consecutive bytes from I2C slave device in burst mode.
 * 
 * Performs a write-restart-read I2C transaction to read multiple consecutive
 * bytes from a starting memory address. The sequence:
 * 1. Waits for I2C bus to be free
 * 2. Sends START condition and slave address with write bit
 * 3. Sends starting memory address
 * 4. Sends RESTART condition
 * 5. Sends slave address with read bit
 * 6. For each byte received:
 *    - Generates ACK for all bytes except the last
 *    - Generates NACK and STOP for the final byte
 * 
 * @param[in] saddr 7-bit slave device address (will be shifted left by 1 for R/W bit)
 * @param[in] maddr Starting memory address to read from
 * @param[in] n Number of bytes to read (must be > 0)
 * @param[out] data Pointer to buffer to store received bytes (must be at least n bytes)
 * 
 * @return void
 * 
 * @note This function is blocking and does not return until all bytes are received
 * @note Suitable for reading from EEPROM and similar devices
 * @note Automatically handles ACK/NACK signaling for burst reads
 * @warning Caller must ensure data buffer has sufficient size for n bytes
 * @warning If n == 0, behavior is undefined; ensure n > 0
 * @see i2c1ByteRead for reading single byte
 */
void i2c1BurstRead(char saddr, char maddr, int n, char* data)
{
    volatile int tmp;

    /*Wait until bus not busy*/
    while(!(I2C1->SR2 & I2C_SR2_BUSY)){}

    /*Generate start*/
    I2C1->CR1 |= I2C_SR1_SB;

    /*Wait until start flag is set*/
    while(!(I2C1->SR1 & I2C_SR1_SB)){}

    /*Transmit slave address + Write*/
    I2C1->DR = (saddr << 1); //Write operation

    /*Wait until address flag is set*/
    while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

    /*Clear addr flag*/
    tmp = I2C1->SR2;

    /*Wait until transmitter empty*/
    while(!(I2C1->SR1 & I2C_SR1_TXE)){}

    /*Send memory address*/
    I2C1->DR = maddr;

    /*Wait until transmit empty*/
    while(!(I2C1->SR1 & I2C_SR1_TXE)){}

    /*Generate restart*/
    I2C1->CR1 |= I2C_CR1_START;

    /*Wait until start flag is set*/
    while(!(I2C1->SR1 & I2C_SR1_SB)){}

    /*Transmit slave address + Read*/
    I2C1->DR = (saddr << 1) | 1; //Read operation

    /*Wait until addr flag is set*/
    while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

    /*Clear addr flag*/
    tmp = I2C1->SR2;

    /*Enable Acknowledge*/
    I2C1->CR1 |= I2C_CR1_ACK;

    while(n > 0U)
    {
        /*if one byte*/
        if(n == 1U)
        {
            /*Disable Acknowledge*/
            I2C1->CR1 &= ~I2C_CR1_ACK;

            /*Generate Stop*/
            I2C1->CR1 |= I2C_CR1_STOP;

            /*Wait for RXNE flag is set*/
            while(!(I2C1->SR1 & I2C_SR1_RXNE)){}

            /*Read data from DR*/
            *data++ = I2C1->DR;
            break;
        }
        else
        {
            /*Wait for RXNE flag is set*/
            while(!(I2C1->SR1 & I2C_SR1_RXNE)){}

            /*Read data from DR*/
            *data++ = I2C1->DR;
            n--;

        }
    }

}


/**
 * @brief Write multiple consecutive bytes to I2C slave device in burst mode.
 * 
 * Performs an I2C write transaction to transmit multiple consecutive bytes
 * to a starting memory address on the slave device. The sequence:
 * 1. Waits for I2C bus to be free
 * 2. Sends START condition and slave address with write bit
 * 3. Sends starting memory address
 * 4. Transmits all n bytes consecutively
 * 5. Waits for byte transfer finished (BTF)
 * 6. Sends STOP condition
 * 
 * @param[in] saddr 7-bit slave device address (will be shifted left by 1 for R/W bit)
 * @param[in] maddr Starting memory address to write to
 * @param[in] n Number of bytes to write (must be > 0)
 * @param[in] data Pointer to buffer containing bytes to write (must have at least n bytes)
 * 
 * @return void
 * 
 * @note This function is blocking and does not return until all bytes are transmitted
 * @note Suitable for writing to EEPROM and similar devices
 * @warning Caller must ensure data buffer has at least n valid bytes
 * @warning If n == 0, behavior is undefined; ensure n > 0
 * @warning Some EEPROM devices have page write limits; writing beyond page boundary may fail
 * @see i2c1BurstRead for reading burst of bytes
 */
void i2c1BurstWrite(char saddr, char maddr, int n, char* data)
{
    volatile int tmp;

    /*Wait until bus not busy*/
    while(!(I2C1->SR2 & I2C_SR2_BUSY)){}

    /*Generate start*/
    I2C1->CR1 |= I2C_SR1_SB;

    /*Wait until start flag is set*/
    while(!(I2C1->SR1 & I2C_SR1_SB)){}

    /*Transmit slave address*/
    I2C1->DR = (saddr << 1); //Write operation

    /*Wait until address flag is set*/
    while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

    /*Clear addr flag*/
    tmp = I2C1->SR2;

    /*Wait until data register empty*/
    while(!(I2C1->SR1 & I2C_SR1_TXE)){}

    /*Send memory address*/
    I2C1->DR = maddr;

    for(int i = 0; i < n; i++)
    {
        /*Wait until data register empty*/
        while(!(I2C1->SR1 & I2C_SR1_TXE)){}

        /*Transmit memory address*/
        I2C1->DR = *data++;
    }

    /*Wait until tranfer finished*/
    while(!(I2C1->SR1 & I2C_SR1_BTF)){}

    /*Generate stop*/
    I2C1->CR1 |= I2C_CR1_STOP;
}