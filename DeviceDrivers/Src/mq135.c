/**
 * @file mq135.c
 * @brief MQ135 Gas Sensor Device Driver Source
 * @version 1.0.0
 */

#include "mq135.h"
#include "bsp_adc.h"
#include <stddef.h>

/* Assuming hadc1 is initialized by CubeMX in main.c and used for MQ135 on PA0 */
extern ADC_HandleTypeDef hadc1;

/* Public functions */

mq135_status_t mq135_init(void)
{
    /* Hardware ADC initialization is handled by CubeMX. 
       This function is a placeholder for any future MQ135-specific startup 
       logic (such as pre-heating delays using bsp_time). */
    return MQ135_OK;
}

mq135_status_t mq135_read_raw(uint32_t *p_raw_value)
{
    if (p_raw_value == NULL)
    {
        return MQ135_ERROR;
    }

    if (bsp_adc_read_raw(&hadc1, p_raw_value) != BSP_ADC_OK)
    {
        return MQ135_ERROR;
    }

    return MQ135_OK;
}

mq135_status_t mq135_read_voltage(float *p_voltage)
{
    if (p_voltage == NULL)
    {
        return MQ135_ERROR;
    }

    if (bsp_adc_read_voltage(&hadc1, p_voltage) != BSP_ADC_OK)
    {
        return MQ135_ERROR;
    }

    return MQ135_OK;
}
