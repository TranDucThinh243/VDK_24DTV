/**
 * @file bsp_i2c.h
 * @brief BSP I2C Module Header
 * @version 1.0.0
 */

#ifndef BSP_I2C_H
#define BSP_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief I2C Status enum
 */
typedef enum {
    BSP_I2C_OK = 0,
    BSP_I2C_ERROR,
    BSP_I2C_BUSY,
    BSP_I2C_TIMEOUT
} bsp_i2c_status_t;

/**
 * @brief Read memory from an I2C device.
 * @param[in] hi2c I2C handle pointer.
 * @param[in] dev_address I2C device address.
 * @param[in] mem_address Register or memory address to read from.
 * @param[in] mem_add_size Size of internal memory address.
 * @param[out] p_data Pointer to the data buffer.
 * @param[in] size Number of bytes to read.
 * @param[in] timeout Timeout duration.
 * @return bsp_i2c_status_t Status of the operation.
 */
bsp_i2c_status_t bsp_i2c_mem_read(I2C_HandleTypeDef *hi2c, uint16_t dev_address, uint16_t mem_address, uint16_t mem_add_size, uint8_t *p_data, uint16_t size, uint32_t timeout);

/**
 * @brief Write memory to an I2C device.
 * @param[in] hi2c I2C handle pointer.
 * @param[in] dev_address I2C device address.
 * @param[in] mem_address Register or memory address to write to.
 * @param[in] mem_add_size Size of internal memory address.
 * @param[in] p_data Pointer to the data buffer.
 * @param[in] size Number of bytes to write.
 * @param[in] timeout Timeout duration.
 * @return bsp_i2c_status_t Status of the operation.
 */
bsp_i2c_status_t bsp_i2c_mem_write(I2C_HandleTypeDef *hi2c, uint16_t dev_address, uint16_t mem_address, uint16_t mem_add_size, uint8_t *p_data, uint16_t size, uint32_t timeout);

/**
 * @brief Transmit data in master mode.
 * @param[in] hi2c I2C handle pointer.
 * @param[in] dev_address I2C device address.
 * @param[in] p_data Pointer to the data buffer to send.
 * @param[in] size Number of bytes to send.
 * @param[in] timeout Timeout duration.
 * @return bsp_i2c_status_t Status of the operation.
 */
bsp_i2c_status_t bsp_i2c_master_transmit(I2C_HandleTypeDef *hi2c, uint16_t dev_address, uint8_t *p_data, uint16_t size, uint32_t timeout);

/**
 * @brief Receive data in master mode.
 * @param[in] hi2c I2C handle pointer.
 * @param[in] dev_address I2C device address.
 * @param[out] p_data Pointer to the data buffer to receive.
 * @param[in] size Number of bytes to receive.
 * @param[in] timeout Timeout duration.
 * @return bsp_i2c_status_t Status of the operation.
 */
bsp_i2c_status_t bsp_i2c_master_receive(I2C_HandleTypeDef *hi2c, uint16_t dev_address, uint8_t *p_data, uint16_t size, uint32_t timeout);

/**
 * @brief Check if an I2C device is ready.
 * @param[in] hi2c I2C handle pointer.
 * @param[in] dev_address I2C device address.
 * @param[in] trials Number of attempts.
 * @param[in] timeout Timeout duration.
 * @return bsp_i2c_status_t Status of the operation.
 */
bsp_i2c_status_t bsp_i2c_is_device_ready(I2C_HandleTypeDef *hi2c, uint16_t dev_address, uint32_t trials, uint32_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* BSP_I2C_H */
