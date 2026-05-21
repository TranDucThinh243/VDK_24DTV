/**
 * @file       bsp_i2c.h
 * @version    1.0.0
 * @date       2026-05-14
 * @author     Tran Duc Thinh
 * @brief      Minimal BSP I2C header
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __BSP_I2C_H
#define __BSP_I2C_H

/* Includes ----------------------------------------------------------- */
#include <stdint.h>
#include "stm32f4xx_hal.h"

/* Public defines ----------------------------------------------------- */
#define BSP_I2C_TIMEOUT_MS    (100U)

/* Public enumerate/structure ----------------------------------------- */
typedef enum
{
  BSP_I2C_OK = 0,
  BSP_I2C_ERROR,
  BSP_I2C_BUSY,
  BSP_I2C_TIMEOUT
}
bsp_i2c_status_t;

/* Public function prototypes ----------------------------------------- */
bsp_i2c_status_t bsp_i2c_from_hal_status(HAL_StatusTypeDef hal_status);

bsp_i2c_status_t bsp_i2c_mem_write(I2C_HandleTypeDef *hi2c,
                                   uint16_t dev_addr,
                                   uint16_t reg_addr,
                                   uint16_t reg_addr_size,
                                   const uint8_t *p_data,
                                   uint16_t size,
                                   uint32_t timeout_ms);

bsp_i2c_status_t bsp_i2c_mem_read(I2C_HandleTypeDef *hi2c,
                                  uint16_t dev_addr,
                                  uint16_t reg_addr,
                                  uint16_t reg_addr_size,
                                  uint8_t *p_data,
                                  uint16_t size,
                                  uint32_t timeout_ms);

void bsp_i2c_delay_ms(uint32_t ms);
uint32_t bsp_i2c_get_tick(void);

#endif /* __BSP_I2C_H */

/* End of file -------------------------------------------------------- */
