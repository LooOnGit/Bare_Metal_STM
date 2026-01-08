/**
 * @file    gpio.c
 * @brief   GPIO driver implementation for STM32F411
 * @author  Loo
 * @version 1.0
 * @date    2026-01-08
 */

#include "gpio.h"

/**
 * @brief Initialize GPIO pins for output and input
 *
 * Enables AHB1 clocks and configures:
 * - PD12 as general purpose output (LED indicator)
 * - PA0 as general purpose input (button/sensor)
 *
 * @return None
 *
 * @see enableRccAHB1Clk()
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

