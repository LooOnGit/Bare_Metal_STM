#include "gpio.h"

/**
 * @brief Initialize GPIO pin
 *
 * Configure a GPIO pin with the given mode and pull configuration.
 *
 */
void GPIO_InitPin(void)
{
    //enable clock for GPIOD
    RCC_AHB1_CLK_ENABLE();

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
void GPIO_WritePin(GPIO_TypeDef *port,
                   uint32_t pin,
                   uint32_t state)
{

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
uint32_t GPIO_ReadPin(GPIO_TypeDef *port,
                      uint32_t pin)
{

}

/**
 * @brief Toggle GPIO pin output
 *
 * @param port Pointer to GPIO port base address
 * @param pin  GPIO pin mask
 */
void GPIO_TogglePin(GPIO_TypeDef *port,
                    uint32_t pin)
{
    
}
