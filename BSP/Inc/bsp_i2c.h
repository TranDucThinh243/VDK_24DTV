/**
 * @file bsp_i2c.h
 * @brief BSP I2C Module Header
 */

#ifndef BSP_I2C_H
#define BSP_I2C_H

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

bsp_i2c_status_t bsp_i2c_mem_read(I2C_HandleTypeDef *hi2c, uint16_t dev_address, uint16_t mem_address, uint16_t mem_add_size, uint8_t *p_data, uint16_t size, uint32_t timeout);
bsp_i2c_status_t bsp_i2c_mem_write(I2C_HandleTypeDef *hi2c, uint16_t dev_address, uint16_t mem_address, uint16_t mem_add_size, uint8_t *p_data, uint16_t size, uint32_t timeout);
bsp_i2c_status_t bsp_i2c_master_transmit(I2C_HandleTypeDef *hi2c, uint16_t dev_address, uint8_t *p_data, uint16_t size, uint32_t timeout);
bsp_i2c_status_t bsp_i2c_master_receive(I2C_HandleTypeDef *hi2c, uint16_t dev_address, uint8_t *p_data, uint16_t size, uint32_t timeout);
bsp_i2c_status_t bsp_i2c_is_device_ready(I2C_HandleTypeDef *hi2c, uint16_t dev_address, uint32_t trials, uint32_t timeout);

void bsp_i2c_delay_ms(uint32_t delay);
uint32_t bsp_i2c_get_tick(void);

#endif /* BSP_I2C_H */
