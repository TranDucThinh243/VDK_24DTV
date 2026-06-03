/**
 * @file bsp_adc.h
 * @brief BSP ADC Module Header
 * @version 1.0.0
 */

#ifndef BSP_ADC_H
#define BSP_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <stdint.h>

#define BSP_ADC_MAX_VALUE      (4095.0f)
#define BSP_ADC_VREF           (3.3f)
#define BSP_ADC_TIMEOUT_MS     (100U)

/**
 * @brief ADC Status enum
 */
typedef enum {
    BSP_ADC_OK = 0,
    BSP_ADC_ERROR,
    BSP_ADC_BUSY,
    BSP_ADC_TIMEOUT
} bsp_adc_status_t;

/**
 * @brief Read raw ADC value from the configured channel.
 * @param[in] hadc ADC handle pointer.
 * @param[out] p_raw_value Pointer to store the raw ADC value.
 * @return bsp_adc_status_t Status of the operation.
 */
bsp_adc_status_t bsp_adc_read_raw(ADC_HandleTypeDef *hadc, uint32_t *p_raw_value);

/**
 * @brief Read and convert ADC value to voltage.
 * @param[in] hadc ADC handle pointer.
 * @param[out] p_voltage Pointer to store the calculated voltage.
 * @return bsp_adc_status_t Status of the operation.
 */
bsp_adc_status_t bsp_adc_read_voltage(ADC_HandleTypeDef *hadc, float *p_voltage);

#ifdef __cplusplus
}
#endif

#endif /* BSP_ADC_H */
