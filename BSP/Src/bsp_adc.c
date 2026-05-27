/**
 * @file bsp_adc.c
 * @brief BSP ADC Module Source
 */

#include "bsp_adc.h"
#include <stddef.h>

/* Private function prototypes */
static bsp_adc_status_t bsp_adc_convert_status(HAL_StatusTypeDef hal_status);

/* Public functions */

bsp_adc_status_t bsp_adc_start(ADC_HandleTypeDef *hadc) {
    if (hadc == NULL) {
        return BSP_ADC_ERROR;
    }
    HAL_StatusTypeDef status = HAL_ADC_Start(hadc);
    return bsp_adc_convert_status(status);
}

bsp_adc_status_t bsp_adc_stop(ADC_HandleTypeDef *hadc) {
    if (hadc == NULL) {
        return BSP_ADC_ERROR;
    }
    HAL_StatusTypeDef status = HAL_ADC_Stop(hadc);
    return bsp_adc_convert_status(status);
}

bsp_adc_status_t bsp_adc_poll(ADC_HandleTypeDef *hadc, uint32_t timeout) {
    if (hadc == NULL) {
        return BSP_ADC_ERROR;
    }
    HAL_StatusTypeDef status = HAL_ADC_PollForConversion(hadc, timeout);
    return bsp_adc_convert_status(status);
}

uint32_t bsp_adc_get_value(ADC_HandleTypeDef *hadc) {
    if (hadc == NULL) {
        return 0;
    }
    return HAL_ADC_GetValue(hadc);
}

bsp_adc_status_t bsp_adc_read_channel_blocking(ADC_HandleTypeDef *hadc, uint32_t *out_value, uint32_t timeout) {
    if (hadc == NULL || out_value == NULL) {
        return BSP_ADC_ERROR;
    }
    
    bsp_adc_status_t status = bsp_adc_start(hadc);
    if (status != BSP_ADC_OK) {
        return status;
    }
    
    status = bsp_adc_poll(hadc, timeout);
    if (status != BSP_ADC_OK) {
        return status;
    }
    
    *out_value = bsp_adc_get_value(hadc);
    
    bsp_adc_stop(hadc);
    
    return BSP_ADC_OK;
}

/* Private functions */

static bsp_adc_status_t bsp_adc_convert_status(HAL_StatusTypeDef hal_status) {
    switch (hal_status) {
        case HAL_OK:
            return BSP_ADC_OK;
        case HAL_ERROR:
            return BSP_ADC_ERROR;
        case HAL_BUSY:
            return BSP_ADC_BUSY;
        case HAL_TIMEOUT:
            return BSP_ADC_TIMEOUT;
        default:
            return BSP_ADC_ERROR;
    }
}
