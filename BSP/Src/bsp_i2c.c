/**
 * @file bsp_i2c.c
 * @brief BSP I2C Module Source
 * @version 1.0.0
 */

#include "bsp_i2c.h"
#include <stddef.h>

/* Private function prototypes */
static bsp_i2c_status_t bsp_i2c_convert_status(HAL_StatusTypeDef hal_status);

/* Public functions */

bsp_i2c_status_t bsp_i2c_mem_read(I2C_HandleTypeDef *hi2c, uint16_t dev_address, uint16_t mem_address, uint16_t mem_add_size, uint8_t *p_data, uint16_t size, uint32_t timeout)
{
    if (hi2c == NULL || p_data == NULL || size == 0U)
    {
        return BSP_I2C_ERROR;
    }
    
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c, dev_address, mem_address, mem_add_size, p_data, size, timeout);
    return bsp_i2c_convert_status(status);
}

bsp_i2c_status_t bsp_i2c_mem_write(I2C_HandleTypeDef *hi2c, uint16_t dev_address, uint16_t mem_address, uint16_t mem_add_size, uint8_t *p_data, uint16_t size, uint32_t timeout)
{
    if (hi2c == NULL || p_data == NULL || size == 0U)
    {
        return BSP_I2C_ERROR;
    }
    
    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(hi2c, dev_address, mem_address, mem_add_size, p_data, size, timeout);
    return bsp_i2c_convert_status(status);
}

bsp_i2c_status_t bsp_i2c_master_transmit(I2C_HandleTypeDef *hi2c, uint16_t dev_address, uint8_t *p_data, uint16_t size, uint32_t timeout)
{
    if (hi2c == NULL || p_data == NULL || size == 0U)
    {
        return BSP_I2C_ERROR;
    }
    
    HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(hi2c, dev_address, p_data, size, timeout);
    return bsp_i2c_convert_status(status);
}

bsp_i2c_status_t bsp_i2c_master_receive(I2C_HandleTypeDef *hi2c, uint16_t dev_address, uint8_t *p_data, uint16_t size, uint32_t timeout)
{
    if (hi2c == NULL || p_data == NULL || size == 0U)
    {
        return BSP_I2C_ERROR;
    }
    
    HAL_StatusTypeDef status = HAL_I2C_Master_Receive(hi2c, dev_address, p_data, size, timeout);
    return bsp_i2c_convert_status(status);
}

bsp_i2c_status_t bsp_i2c_is_device_ready(I2C_HandleTypeDef *hi2c, uint16_t dev_address, uint32_t trials, uint32_t timeout)
{
    if (hi2c == NULL)
    {
        return BSP_I2C_ERROR;
    }
    
    HAL_StatusTypeDef status = HAL_I2C_IsDeviceReady(hi2c, dev_address, trials, timeout);
    return bsp_i2c_convert_status(status);
}

/* Private functions */

static bsp_i2c_status_t bsp_i2c_convert_status(HAL_StatusTypeDef hal_status)
{
    switch (hal_status)
    {
        case HAL_OK:
            return BSP_I2C_OK;
        case HAL_ERROR:
            return BSP_I2C_ERROR;
        case HAL_BUSY:
            return BSP_I2C_BUSY;
        case HAL_TIMEOUT:
            return BSP_I2C_TIMEOUT;
        default:
            return BSP_I2C_ERROR;
    }
}
