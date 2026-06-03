/**
 * @file bsp_adc.c
 * @brief BSP ADC Module Source
 * @version 1.0.0
 */

#include "bsp_adc.h"
#include <stddef.h>

/* Private function prototypes */
static bsp_adc_status_t bsp_adc_convert_status(HAL_StatusTypeDef hal_status);

/* Public functions */

bsp_adc_status_t bsp_adc_read_raw(ADC_HandleTypeDef *hadc, uint32_t *p_raw_value)
{
    if (hadc == NULL || p_raw_value == NULL)
    {
        return BSP_ADC_ERROR;
    }

    HAL_StatusTypeDef status = HAL_ADC_Start(hadc);
    if (status != HAL_OK)
    {
        return bsp_adc_convert_status(status);
    }

    status = HAL_ADC_PollForConversion(hadc, BSP_ADC_TIMEOUT_MS);
    if (status == HAL_OK)
    {
        *p_raw_value = HAL_ADC_GetValue(hadc);
    }
    
    HAL_ADC_Stop(hadc);

    return bsp_adc_convert_status(status);
}

bsp_adc_status_t bsp_adc_read_voltage(ADC_HandleTypeDef *hadc, float *p_voltage)
{
    if (hadc == NULL || p_voltage == NULL)
    {
        return BSP_ADC_ERROR;
    }

    uint32_t raw_value = 0;
    bsp_adc_status_t status = bsp_adc_read_raw(hadc, &raw_value);
    
    if (status == BSP_ADC_OK)
    {
        *p_voltage = ((float)raw_value / BSP_ADC_MAX_VALUE) * BSP_ADC_VREF;
    }

    return status;
}

/* Private functions */

static bsp_adc_status_t bsp_adc_convert_status(HAL_StatusTypeDef hal_status)
{
    switch (hal_status)
    {
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
