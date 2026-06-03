/**
 * @file air_quality_service.c
 * @brief Air Quality Service Module Source
 * @version 1.0.0
 */

#include "air_quality_service.h"
#include "mq135.h"
#include <stddef.h>

/* Simple thresholds for air quality levels based on delta PPM */
#define AIR_QUALITY_DELTA_THRESHOLD_WARNING  (50U)
#define AIR_QUALITY_DELTA_THRESHOLD_DANGER   (80U)
#define AIR_QUALITY_CALIBRATION_SAMPLES      (5U)

/* Static calibration state */
static uint32_t s_calibration_sum = 0;
static uint8_t s_calibration_count = 0;
static uint8_t s_is_calibrated = 0;

/* Static variable holding the latest processed data */
static air_quality_data_t s_latest_data = {
    .raw_adc = 0,
    .voltage_pa0 = 0.0f,
    .voltage_sensor = 0.0f,
    .ppm_est = 0,
    .baseline_ppm_est = 0,
    .delta_ppm = 0,
    .level = AIR_QUALITY_GOOD,
    .sensor_ok = 0
};

/* Public functions */

air_quality_service_status_t air_quality_service_init(void)
{
    s_calibration_sum = 0;
    s_calibration_count = 0;
    s_is_calibrated = 0;

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
    float voltage_pa0 = 0.0f;

    /* Read raw ADC from driver */
    if (mq135_read_raw(&raw_val) != MQ135_OK)
    {
        s_latest_data.sensor_ok = 0;
        return AIR_QUALITY_SERVICE_ERROR;
    }

    /* Read voltage from driver */
    if (mq135_read_voltage(&voltage_pa0) != MQ135_OK)
    {
        s_latest_data.sensor_ok = 0;
        return AIR_QUALITY_SERVICE_ERROR;
    }

    /* Voltage divider correction: R1=1.8k, R2=3.3k */
    float voltage_sensor = voltage_pa0 * (1.8f + 3.3f) / 3.3f;

    /* Prevent division by zero and limit maximums */
    if (voltage_sensor < 0.01f) voltage_sensor = 0.01f;
    if (voltage_sensor > 4.99f) voltage_sensor = 4.99f;

    /* Estimate relative PPM using inverse resistance (gas up -> Rs down -> PPM up) */
    float rs_factor = (5.0f - voltage_sensor) / voltage_sensor;
    uint32_t current_ppm = (uint32_t)(10000.0f / rs_factor);

    /* Process baseline calibration */
    if (!s_is_calibrated)
    {
        s_calibration_sum += current_ppm;
        s_calibration_count++;
        
        if (s_calibration_count >= AIR_QUALITY_CALIBRATION_SAMPLES)
        {
            s_latest_data.baseline_ppm_est = s_calibration_sum / AIR_QUALITY_CALIBRATION_SAMPLES;
            s_is_calibrated = 1;
        }
        
        s_latest_data.delta_ppm = 0;
        s_latest_data.level = AIR_QUALITY_GOOD;
    }
    else
    {
        /* Calculate delta from baseline */
        if (current_ppm > s_latest_data.baseline_ppm_est)
        {
            s_latest_data.delta_ppm = current_ppm - s_latest_data.baseline_ppm_est;
        }
        else
        {
            s_latest_data.delta_ppm = 0;
        }

        /* Classify based on delta */
        if (s_latest_data.delta_ppm >= AIR_QUALITY_DELTA_THRESHOLD_DANGER)
        {
            s_latest_data.level = AIR_QUALITY_DANGER;
        }
        else if (s_latest_data.delta_ppm >= AIR_QUALITY_DELTA_THRESHOLD_WARNING)
        {
            s_latest_data.level = AIR_QUALITY_WARNING;
        }
        else
        {
            s_latest_data.level = AIR_QUALITY_GOOD;
        }
    }

    /* Update internal static state */
    s_latest_data.sensor_ok = 1;
    s_latest_data.raw_adc = (uint16_t)raw_val;
    s_latest_data.voltage_pa0 = voltage_pa0;
    s_latest_data.voltage_sensor = voltage_sensor;
    s_latest_data.ppm_est = current_ppm;

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
