/**
 * @file bsp_pwm.h
 * @brief BSP PWM Module Header
 */

#ifndef BSP_PWM_H
#define BSP_PWM_H

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief PWM Status enum
 */
typedef enum {
    BSP_PWM_OK = 0,
    BSP_PWM_ERROR
} bsp_pwm_status_t;

bsp_pwm_status_t bsp_pwm_start(TIM_HandleTypeDef *htim, uint32_t channel);
bsp_pwm_status_t bsp_pwm_stop(TIM_HandleTypeDef *htim, uint32_t channel);

/**
 * @brief Set the duty cycle for a specific PWM channel
 * @param htim Timer handle
 * @param channel Timer channel
 * @param duty_percent Duty cycle percentage (0.0 to 100.0)
 * @return bsp_pwm_status_t Status of the operation
 */
bsp_pwm_status_t bsp_pwm_set_duty(TIM_HandleTypeDef *htim, uint32_t channel, float duty_percent);

#endif /* BSP_PWM_H */
