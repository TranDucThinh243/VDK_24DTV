/**
 * @file hc05.h
 * @brief HC-05 Bluetooth Device Driver Header
 * @version 1.0.0
 */

#ifndef HC05_H
#define HC05_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief HC-05 status enum
 */
typedef enum {
    HC05_OK = 0,
    HC05_ERROR,
    HC05_BUSY,
    HC05_TIMEOUT
} hc05_status_t;

/**
 * @brief Initialize the HC-05 Bluetooth module.
 * @return hc05_status_t Status of initialization.
 */
hc05_status_t hc05_init(void);

/**
 * @brief Send an array of bytes over Bluetooth.
 * @param[in] p_data Pointer to the data to send.
 * @param[in] size Number of bytes to send.
 * @return hc05_status_t Status of the operation.
 */
hc05_status_t hc05_send_bytes(uint8_t *p_data, uint16_t size);

/**
 * @brief Send a null-terminated string over Bluetooth.
 * @param[in] str Pointer to the null-terminated string.
 * @return hc05_status_t Status of the operation.
 */
hc05_status_t hc05_send_string(const char *str);

/**
 * @brief Receive an array of bytes over Bluetooth.
 * @param[out] p_data Pointer to the buffer to store received data.
 * @param[in] size Number of bytes to receive.
 * @return hc05_status_t Status of the operation.
 */
hc05_status_t hc05_receive_bytes(uint8_t *p_data, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif /* HC05_H */
