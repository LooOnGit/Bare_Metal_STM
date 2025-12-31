#ifndef __GPIO__H__
#define __GPIO__H__
/**
 * @file    gpio.h
 * @brief   GPIO low-level driver for STM32F411
 *
 * This driver provides basic GPIO control functions such as
 * clock enable, pin initialization, read/write and toggle.
 * It is implemented using CMSIS registers (no HAL dependency).
 */
#define STM32F411xE
#include "stm32f4xx.h" 

/**
 * @defgroup GPIO GPIO Driver
 * @brief Low-level GPIO driver
 * @{
 */

/**
 * @brief Enable peripheral clock for GPIO port
 *
 * This function enables the AHB1 clock for the given GPIO port.
 *
 * @param port Pointer to GPIO port base address
 *             (e.g. GPIOA, GPIOB, GPIOD)
 *
 * @note This function must be called before any GPIO configuration.
 */
void GPIO_EnableClock(GPIO_TypeDef *port);

/**
 * @brief Initialize GPIO pin
 *
 * Configure a GPIO pin with the given mode and pull configuration.
 *
 * @param port Pointer to GPIO port base address
 * @param pin  GPIO pin mask (e.g. (1U << 12))
 * @param mode GPIO mode
 *             (e.g. GPIO_MODE_INPUT, GPIO_MODE_OUTPUT,
 *              GPIO_MODE_AF, GPIO_MODE_ANALOG)
 * @param pull GPIO pull configuration
 *             (e.g. GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN)
 *
 * @note GPIO speed and output type are not configured in this function.
 */
void GPIO_InitPin(GPIO_TypeDef *port,
                  uint32_t pin,
                  uint32_t mode,
                  uint32_t pull);

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
                   uint32_t state);

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
                      uint32_t pin);

/**
 * @brief Toggle GPIO pin output
 *
 * @param port Pointer to GPIO port base address
 * @param pin  GPIO pin mask
 */
void GPIO_TogglePin(GPIO_TypeDef *port,
                    uint32_t pin);

/** @} */ /* end of GPIO */


#endif // __GPIO__H__
