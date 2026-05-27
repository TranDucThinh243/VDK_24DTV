/**
 * @file bsp_gpio.c
 * @brief BSP GPIO Module Source
 */

#include "bsp_gpio.h"
#include <stddef.h>

/* Public functions */

void bsp_gpio_pin_write(GPIO_TypeDef *port, uint16_t pin, bsp_gpio_state_t state) {
    if (port == NULL) {
        return;
    }
    HAL_GPIO_WritePin(port, pin, (state == BSP_GPIO_PIN_SET) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void bsp_gpio_pin_toggle(GPIO_TypeDef *port, uint16_t pin) {
    if (port == NULL) {
        return;
    }
    HAL_GPIO_TogglePin(port, pin);
}

bsp_gpio_state_t bsp_gpio_pin_read(GPIO_TypeDef *port, uint16_t pin) {
    if (port == NULL) {
        return BSP_GPIO_PIN_RESET;
    }
    GPIO_PinState state = HAL_GPIO_ReadPin(port, pin);
    return (state == GPIO_PIN_SET) ? BSP_GPIO_PIN_SET : BSP_GPIO_PIN_RESET;
}
