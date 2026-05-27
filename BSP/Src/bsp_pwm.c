/**
 * @file bsp_pwm.c
 * @brief BSP PWM Module Source
 */

#include "bsp_pwm.h"
#include <stddef.h>

/* Public functions */

bsp_pwm_status_t bsp_pwm_start(TIM_HandleTypeDef *htim, uint32_t channel) {
    if (htim == NULL) {
        return BSP_PWM_ERROR;
    }
    if (HAL_TIM_PWM_Start(htim, channel) != HAL_OK) {
        return BSP_PWM_ERROR;
    }
    return BSP_PWM_OK;
}

bsp_pwm_status_t bsp_pwm_stop(TIM_HandleTypeDef *htim, uint32_t channel) {
    if (htim == NULL) {
        return BSP_PWM_ERROR;
    }
    if (HAL_TIM_PWM_Stop(htim, channel) != HAL_OK) {
        return BSP_PWM_ERROR;
    }
    return BSP_PWM_OK;
}

bsp_pwm_status_t bsp_pwm_set_duty(TIM_HandleTypeDef *htim, uint32_t channel, float duty_percent) {
    if (htim == NULL) {
        return BSP_PWM_ERROR;
    }
    
    if (duty_percent < 0.0f) {
        duty_percent = 0.0f;
    } else if (duty_percent > 100.0f) {
        duty_percent = 100.0f;
    }
    
    uint32_t period = __HAL_TIM_GET_AUTORELOAD(htim);
    uint32_t compare_value = (uint32_t)((duty_percent / 100.0f) * (float)(period + 1));
    
    __HAL_TIM_SET_COMPARE(htim, channel, compare_value);
    
    return BSP_PWM_OK;
}
