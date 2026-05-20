/**
 * @file       bsp_i2c.c
 * @version    1.0.0
 * @date       2026-05-14
 *
 * @brief      Minimal BSP I2C source template
 */

/* Includes ----------------------------------------------------------- */
#include "bsp_i2c.h"

/* Public function definitions ---------------------------------------- */
bsp_i2c_status_t bsp_i2c_from_hal_status(HAL_StatusTypeDef hal_status)
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
bsp_i2c_status_t bsp_i2c_mem_write(I2C_HandleTypeDef *hi2c,
                                   uint16_t dev_addr,
                                   uint16_t reg_addr,
                                   uint16_t reg_addr_size,
                                   const uint8_t *p_data,
                                   uint16_t size,
                                   uint32_t timeout_ms)
{
  if(hi2c == NULL || p_data == NULL || size == 0U)
  {
    return BSP_I2C_ERROR;
  }
  
  HAL_StatusTypeDef hal_status = HAL_I2C_Mem_Write(hi2c, dev_addr, reg_addr, reg_addr_size, (uint8_t *)p_data, size, timeout_ms);
  
  return bsp_i2c_from_hal_status(hal_status);
}

bsp_i2c_status_t bsp_i2c_mem_read(I2C_HandleTypeDef *hi2c,
                                  uint16_t dev_addr,
                                  uint16_t reg_addr,
                                  uint16_t reg_addr_size,
                                  uint8_t *p_data,
                                  uint16_t size,
                                  uint32_t timeout_ms)
{
  if(hi2c == NULL || p_data == NULL || size == 0U)
  {
    return BSP_I2C_ERROR;
  }

  HAL_StatusTypeDef hal_status = HAL_I2C_Mem_Read(hi2c, dev_addr, reg_addr, reg_addr_size, p_data, size, timeout_ms);
  
  return bsp_i2c_from_hal_status(hal_status);
}

void bsp_i2c_delay_ms(uint32_t ms)
{
  
  HAL_Delay(ms);

}

uint32_t bsp_i2c_get_tick(void)
{

  return HAL_GetTick();

}
