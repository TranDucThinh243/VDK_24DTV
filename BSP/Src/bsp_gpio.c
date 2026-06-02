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

void bsp_gpio_init(void) {
    // Enable GPIO clocks
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // Configure PA8 as GPIO Output
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // Configure PB12, PB13, PB14 as GPIO Input with internal pull-up
    GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // Initial state setup for PA8 (LED OFF)
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
}
