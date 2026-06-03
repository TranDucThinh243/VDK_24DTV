/**
 * @file air_quality_service.c
 * @brief Air Quality Service Module Source
 * @version 1.0.0
 */

#include "air_quality_service.h"
#include "mq135.h"
#include <stddef.h>

/* Simple voltage thresholds for air quality levels */
#define AIR_QUALITY_VOLTAGE_THRESHOLD_WARNING  (1.5f)
#define AIR_QUALITY_VOLTAGE_THRESHOLD_DANGER   (2.5f)

/* Static variable holding the latest processed data */
static air_quality_data_t s_latest_data = {
    .raw_adc = 0,
    .voltage = 0.0f,
    .level = AIR_QUALITY_GOOD,
    .sensor_ok = 0
};

/* Public functions */

air_quality_service_status_t air_quality_service_init(void)
{
    if (mq135_init() != MQ135_OK)
    {
        s_latest_data.sensor_ok = 0;
        return AIR_QUALITY_SERVICE_ERROR;
    }
    
    s_latest_data.sensor_ok = 1;
    return AIR_QUALITY_SERVICE_OK;
}

air_quality_service_status_t air_quality_service_update(void)
{
    uint32_t raw_val = 0;
    float voltage_val = 0.0f;

    /* Read raw ADC from driver */
    if (mq135_read_raw(&raw_val) != MQ135_OK)
    {
        s_latest_data.sensor_ok = 0;
        return AIR_QUALITY_SERVICE_ERROR;
    }

    /* Read voltage from driver */
    if (mq135_read_voltage(&voltage_val) != MQ135_OK)
    {
        s_latest_data.sensor_ok = 0;
        return AIR_QUALITY_SERVICE_ERROR;
    }

    /* Update internal static state */
    s_latest_data.sensor_ok = 1;
    s_latest_data.raw_adc = (uint16_t)raw_val;
    s_latest_data.voltage = voltage_val;

    /* Classify air quality level based on voltage thresholds */
    if (voltage_val >= AIR_QUALITY_VOLTAGE_THRESHOLD_DANGER)
    {
        s_latest_data.level = AIR_QUALITY_DANGER;
    }
    else if (voltage_val >= AIR_QUALITY_VOLTAGE_THRESHOLD_WARNING)
    {
        s_latest_data.level = AIR_QUALITY_WARNING;
    }
    else
    {
        s_latest_data.level = AIR_QUALITY_GOOD;
    }

    return AIR_QUALITY_SERVICE_OK;
}

air_quality_service_status_t air_quality_service_get_data(air_quality_data_t *p_data)
{
    if (p_data == NULL)
    {
        return AIR_QUALITY_SERVICE_ERROR;
    }

    /* Copy latest data into provided pointer */
    *p_data = s_latest_data;

    return AIR_QUALITY_SERVICE_OK;
}
