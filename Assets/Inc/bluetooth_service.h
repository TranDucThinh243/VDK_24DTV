/**
 * @file bluetooth_service.h
 * @brief Bluetooth Service Module Header
 * @version 1.0.0
 */

#ifndef BLUETOOTH_SERVICE_H
#define BLUETOOTH_SERVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "air_quality_service.h"

/**
 * @brief Bluetooth service status enum
 */
typedef enum {
    BLUETOOTH_SERVICE_OK = 0,
    BLUETOOTH_SERVICE_ERROR
} bluetooth_service_status_t;

/**
 * @brief Initialize the Bluetooth service and underlying driver.
 * @return bluetooth_service_status_t Status of initialization.
 */
bluetooth_service_status_t bluetooth_service_init(void);

/**
 * @brief Format and send the air quality data over Bluetooth.
 * @param[in] p_data Pointer to the air quality data to send.
 * @return bluetooth_service_status_t Status of the operation.
 */
bluetooth_service_status_t bluetooth_service_send_air_quality(const air_quality_data_t *p_data);

/**
 * @brief Send a raw string over Bluetooth.
 * @param[in] str Null-terminated string to send.
 * @return bluetooth_service_status_t Status of the operation.
 */
bluetooth_service_status_t bluetooth_service_send_string(const char *str);

/**
 * @brief Get the current status of the Bluetooth service/driver.
 * @return bluetooth_service_status_t Status of the module.
 */
bluetooth_service_status_t bluetooth_service_get_status(void);

#ifdef __cplusplus
}
#endif

#endif /* BLUETOOTH_SERVICE_H */
