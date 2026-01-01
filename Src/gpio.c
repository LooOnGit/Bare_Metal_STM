#include "gpio.h"

/**
 * @brief Initialize GPIO pin
 *
 * Configure a GPIO pin with the given mode and pull configuration.
 *
 */
void initGPIOPin(void)
{
    //enable clock for GPIOD
    enableRccAHB1Clk();

    // set mode D12 to output
    GPIOD->MODER &= ~GPIO_MODER_MODER12; // Clear mode bits for pin 12
    GPIOD->MODER |= GPIO_MODER_MODER12_0;

    //set mode A0 to input
    GPIOA->MODER &= ~GPIO_MODER_MODER0; // Clear mode bits for pin 0

}

/**
 * @brief Write logic level to GPIO pin
 *
 * Set or reset the selected GPIO pin.
 *
 * @param port  Pointer to GPIO port base address
 * @param pin   GPIO pin mask
 * @param state Pin state
 *              (0 = reset, non-zero = set)
 */
void writeGPIOPin(uint32_t state)
{
    GPIOD->ODR &= ~ GPIO_ODR_OD12;
    if (state == ON)
    {
        GPIOD->ODR |= GPIO_ODR_OD12;
    }
}

/**
 * @brief Write logic level to GPIO pin
 *
 * @note This function uses the GPIO BSRR register for atomic bit
 *       manipulation, ensuring safe operation in interrupt-driven
 *       and multi-context environments.
 *
 * @param state Pin state (ON/OFF)
 */
void writeGPIOPinBSSR(uint32_t state)
{
    if(state == ON)
    {
        GPIOD->BSRR |= GPIO_BSRR_BS12;         //set pin D12
    }else
    {
        GPIOD->BSRR |= GPIO_BSRR_BR12;        //reset pin D12
    }
}

/**
 * @brief Read logic level from GPIO pin
 *
 * @param port Pointer to GPIO port base address
 * @param pin  GPIO pin mask
 *
 * @return Pin state
 *         - 0: logic low
 *         - 1: logic high
 */
bool readGPIOPin(uint32_t pin)
{
    if(GPIOA->IDR & GPIO_IDR_ID0)
    {
        return false;
    }else
    {
        return true;
    }
}

