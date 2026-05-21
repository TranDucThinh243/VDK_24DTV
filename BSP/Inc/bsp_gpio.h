/*
 * @file       bsp_gpio.h
 * @version    1.0.0
 * @date       2026-05-21
 *
 * @brief      Minimal BSP GPIO interface
 */

#ifndef __BSP_GPIO_H
#define __BSP_GPIO_H

/* Includes ----------------------------------------------------------- */
#include <stdint.h>
#include "stm32f1xx_hal.h"

/* Public defines ----------------------------------------------------- */
#define BSP_GPIO_LED_PORT    GPIOA
#define BSP_GPIO_LED_PIN     GPIO_PIN_8

#define BSP_GPIO_KEY1_PORT   GPIOB
#define BSP_GPIO_KEY1_PIN    GPIO_PIN_12

#define BSP_GPIO_KEY2_PORT   GPIOB
#define BSP_GPIO_KEY2_PIN    GPIO_PIN_13

#define BSP_GPIO_KEY3_PORT   GPIOB
#define BSP_GPIO_KEY3_PIN    GPIO_PIN_14

/* Public enumerate/structure ---------------------------------------- */
typedef enum
{
  BSP_GPIO_OK = 0U,
  BSP_GPIO_ERROR
}
bsp_gpio_status_t;

/* Public function prototypes ----------------------------------------- */
void bsp_gpio_init(void);

bsp_gpio_status_t bsp_gpio_config_input(GPIO_TypeDef *gpio_port,
                                        uint16_t gpio_pin,
                                        uint32_t pull);

bsp_gpio_status_t bsp_gpio_config_output(GPIO_TypeDef *gpio_port,
                                         uint16_t gpio_pin,
                                         GPIO_PinState initial_state,
                                         uint32_t pull,
                                         uint32_t speed);

bsp_gpio_status_t bsp_gpio_write(GPIO_TypeDef *gpio_port,
                                 uint16_t gpio_pin,
                                 GPIO_PinState pin_state);

bsp_gpio_status_t bsp_gpio_toggle(GPIO_TypeDef *gpio_port,
                                  uint16_t gpio_pin);

GPIO_PinState bsp_gpio_read(GPIO_TypeDef *gpio_port,
                            uint16_t gpio_pin);

#endif /* __BSP_GPIO_H */