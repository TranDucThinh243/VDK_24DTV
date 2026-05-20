/**
 * @file       bsp_i2c.h
 * @version    1.0.0
 * @date       2026-05-14
 *
 * @brief      Minimal BSP I2C interface
 * @note       Simple BSP functions for register-based I2C devices.
 */

#ifndef __BSP_I2C_H
#define __BSP_I2C_H

/* Includes ----------------------------------------------------------- */
#include <stdint.h>
#include "stm32f1xx_hal.h"

/* Public defines ----------------------------------------------------- */
#ifndef BSP_I2C_TIMEOUT_MS
#define BSP_I2C_TIMEOUT_MS    (100U)
#endif

/* Public enumerate/structure ----------------------------------------- */
typedef enum
{
  BSP_I2C_OK = 0U,
  BSP_I2C_ERROR,
  BSP_I2C_TIMEOUT,
  BSP_I2C_BUSY
}
bsp_i2c_status_t;

/* Public function prototypes ----------------------------------------- */
/**
 * @brief  Write data to a device register through I2C
 *
 * @param[in] hi2c           Pointer to I2C handle
 * @param[in] dev_addr       Device address
 * @param[in] reg_addr       Register address inside slave
 * @param[in] reg_addr_size  Register address size
 * @param[in] p_data         Data buffer to write
 * @param[in] size           Number of bytes to write
 * @param[in] timeout_ms     Timeout in milliseconds
 *
 * @return BSP I2C status
 */
bsp_i2c_status_t bsp_i2c_mem_write(I2C_HandleTypeDef *hi2c,
                                   uint16_t dev_addr,
                                   uint16_t reg_addr,
                                   uint16_t reg_addr_size,
                                   const uint8_t *p_data,
                                   uint16_t size,
                                   uint32_t timeout_ms);

/**
 * @brief  Read data from a device register through I2C
 *
 * @param[in]  hi2c           Pointer to I2C handle
 * @param[in]  dev_addr       Device address
 * @param[in]  reg_addr       Register address inside slave
 * @param[in]  reg_addr_size  Register address size
 * @param[out] p_data         Data buffer to receive
 * @param[in]  size           Number of bytes to read
 * @param[in]  timeout_ms     Timeout in milliseconds
 *
 * @return BSP I2C status
 */
bsp_i2c_status_t bsp_i2c_mem_read(I2C_HandleTypeDef *hi2c,
                                  uint16_t dev_addr,
                                  uint16_t reg_addr,
                                  uint16_t reg_addr_size,
                                  uint8_t *p_data,
                                  uint16_t size,
                                  uint32_t timeout_ms);

/**
 * @brief  Delay in milliseconds
 *
 * @param[in] ms Delay time in milliseconds
 */
void bsp_i2c_delay_ms(uint32_t ms);

/**
 * @brief  Get system tick in milliseconds
 *
 * @return Current tick in milliseconds
 */
uint32_t bsp_i2c_get_tick(void);
/**
 * @brief  Convert HAL status to BSP I2C status
 *
 * @param[in] hal_status HAL status code
 *
 * @return Corresponding BSP I2C status
 */
bsp_i2c_status_t bsp_i2c_from_hal_status(HAL_StatusTypeDef hal_status);
#endif /* __BSP_I2C_H */
