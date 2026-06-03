/**
 * @file air_quality_service.h
 * @brief Air Quality Service Module Header
 * @version 1.0.0
 */

#ifndef AIR_QUALITY_SERVICE_H
#define AIR_QUALITY_SERVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Air quality level categories based on sensor voltage
 */
typedef enum {
    AIR_QUALITY_GOOD = 0,
    AIR_QUALITY_WARNING,
    AIR_QUALITY_DANGER
} air_quality_level_t;

/**
 * @brief Air quality service status enum
 */
typedef enum {
    AIR_QUALITY_SERVICE_OK = 0,
    AIR_QUALITY_SERVICE_ERROR
} air_quality_service_status_t;

/**
 * @brief Processed air quality data structure
 */
typedef struct {
    uint16_t raw_adc;
    float voltage;
    air_quality_level_t level;
    uint8_t sensor_ok;
} air_quality_data_t;

/**
 * @brief Initialize the air quality service and underlying driver.
 * @return air_quality_service_status_t Status of initialization.
 */
air_quality_service_status_t air_quality_service_init(void);

/**
 * @brief Update the air quality data by reading from the sensor.
 * @return air_quality_service_status_t Status of the update operation.
 */
air_quality_service_status_t air_quality_service_update(void);

/**
 * @brief Get the latest processed air quality data.
 * @param[out] p_data Pointer to a structure to store the data.
 * @return air_quality_service_status_t Status of the operation.
 */
air_quality_service_status_t air_quality_service_get_data(air_quality_data_t *p_data);

#ifdef __cplusplus
}
#endif

#endif /* AIR_QUALITY_SERVICE_H */
