/**
 * @file bsp_adc.h
 * @brief BSP ADC Module Header
 */

#ifndef BSP_ADC_H
#define BSP_ADC_H

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief ADC Status enum
 */
typedef enum {
    BSP_ADC_OK = 0,
    BSP_ADC_ERROR,
    BSP_ADC_BUSY,
    BSP_ADC_TIMEOUT
} bsp_adc_status_t;

bsp_adc_status_t bsp_adc_start(ADC_HandleTypeDef *hadc);
bsp_adc_status_t bsp_adc_stop(ADC_HandleTypeDef *hadc);
bsp_adc_status_t bsp_adc_poll(ADC_HandleTypeDef *hadc, uint32_t timeout);
uint32_t bsp_adc_get_value(ADC_HandleTypeDef *hadc);
bsp_adc_status_t bsp_adc_read_channel_blocking(ADC_HandleTypeDef *hadc, uint32_t *out_value, uint32_t timeout);

#endif /* BSP_ADC_H */
