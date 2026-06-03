/**
 * @file bluetooth_service.c
 * @brief Bluetooth Service Module Source
 * @version 1.0.0
 */

#include "bluetooth_service.h"
#include "hc05.h"
#include <stddef.h>
#include <stdio.h>

/* Static variables */
static bluetooth_service_status_t s_module_status = BLUETOOTH_SERVICE_OK;

/* Public functions */

bluetooth_service_status_t bluetooth_service_init(void)
{
    if (hc05_init() != HC05_OK)
    {
        s_module_status = BLUETOOTH_SERVICE_ERROR;
        return BLUETOOTH_SERVICE_ERROR;
    }
    
    s_module_status = BLUETOOTH_SERVICE_OK;
    return BLUETOOTH_SERVICE_OK;
}

bluetooth_service_status_t bluetooth_service_send_air_quality(const air_quality_data_t *p_data)
{
    char buf[64];
    const char *lvl_str;

    if (p_data == NULL)
    {
        return BLUETOOTH_SERVICE_ERROR;
    }

    if (!p_data->sensor_ok)
    {
        return bluetooth_service_send_string("AIR:ERROR\r\n");
    }

    switch (p_data->level)
    {
        case AIR_QUALITY_GOOD:
            lvl_str = "GOOD";
            break;
        case AIR_QUALITY_WARNING:
            lvl_str = "WARN";
            break;
        case AIR_QUALITY_DANGER:
            lvl_str = "DANGER";
            break;
        default:
            lvl_str = "UNKNOWN";
            break;
    }

    /* Format text payload: AIR:GOOD,MQ:1.48,RAW:1840 */
    /* Use integer math to safely print floats to avoid soft-float printf issues */
    int volts_int = (int)p_data->voltage;
    int volts_frac = (int)(p_data->voltage * 100.0f) % 100;

    snprintf(buf, sizeof(buf), "AIR:%s,MQ:%d.%02d,RAW:%u\r\n", 
             lvl_str, volts_int, volts_frac, p_data->raw_adc);

    if (hc05_send_string(buf) != HC05_OK)
    {
        s_module_status = BLUETOOTH_SERVICE_ERROR;
        return BLUETOOTH_SERVICE_ERROR;
    }

    s_module_status = BLUETOOTH_SERVICE_OK;
    return BLUETOOTH_SERVICE_OK;
}

bluetooth_service_status_t bluetooth_service_send_string(const char *str)
{
    if (str == NULL)
    {
        return BLUETOOTH_SERVICE_ERROR;
    }

    if (hc05_send_string(str) != HC05_OK)
    {
        s_module_status = BLUETOOTH_SERVICE_ERROR;
        return BLUETOOTH_SERVICE_ERROR;
    }

    s_module_status = BLUETOOTH_SERVICE_OK;
    return BLUETOOTH_SERVICE_OK;
}

bluetooth_service_status_t bluetooth_service_get_status(void)
{
    return s_module_status;
}
