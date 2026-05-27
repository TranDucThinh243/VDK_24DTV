/**
 * @file bsp_gpio.h
 * @brief BSP GPIO Module Header
 */

#ifndef BSP_GPIO_H
#define BSP_GPIO_H

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief GPIO State enum
 */
typedef enum {
    BSP_GPIO_PIN_RESET = 0,
    BSP_GPIO_PIN_SET
} bsp_gpio_state_t;

void bsp_gpio_pin_write(GPIO_TypeDef *port, uint16_t pin, bsp_gpio_state_t state);
void bsp_gpio_pin_toggle(GPIO_TypeDef *port, uint16_t pin);
bsp_gpio_state_t bsp_gpio_pin_read(GPIO_TypeDef *port, uint16_t pin);

#endif /* BSP_GPIO_H */