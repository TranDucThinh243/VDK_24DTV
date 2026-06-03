/**
 * @file mq135.h
 * @brief MQ135 Gas Sensor Device Driver Header
 * @version 1.0.0
 */

#ifndef MQ135_H
#define MQ135_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief MQ135 status enum
 */
typedef enum {
    MQ135_OK = 0,
    MQ135_ERROR
} mq135_status_t;

/**
 * @brief Initialize the MQ135 sensor driver.
 * @return mq135_status_t Status of initialization.
 */
mq135_status_t mq135_init(void);

/**
 * @brief Read the raw ADC value from the MQ135 sensor.
 * @param[out] p_raw_value Pointer to store the raw ADC value.
 * @return mq135_status_t Status of the operation.
 */
mq135_status_t mq135_read_raw(uint32_t *p_raw_value);

/**
 * @brief Read the converted voltage from the MQ135 sensor.
 * @param[out] p_voltage Pointer to store the calculated voltage.
 * @return mq135_status_t Status of the operation.
 */
mq135_status_t mq135_read_voltage(float *p_voltage);

#ifdef __cplusplus
}
#endif

#endif /* MQ135_H */
