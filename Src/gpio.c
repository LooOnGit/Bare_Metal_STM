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

    // set mode
    GPIOD->MODER &= ~GPIO_MODER_MODER12; // Clear mode bits for pin 12
    GPIOD->MODER |= GPIO_MODER_MODER12_0;
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
 * @brief Read logic level from GPIO pin
 *
 * @param port Pointer to GPIO port base address
 * @param pin  GPIO pin mask
 *
 * @return Pin state
 *         - 0: logic low
 *         - 1: logic high
 */
uint32_t readGPIOPin(
uint32_t pin)
{

}
