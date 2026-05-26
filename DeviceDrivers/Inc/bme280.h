/**
 * @file       bme280.h
 * @version    1.0.0
 * @date       2026-05-22
 * @author     Tran Duc Thinh
 * @brief      Portable BME280 I2C driver interface
 */

#ifndef __BME280_H
#define __BME280_H

#include <stdint.h>
#include "bsp_i2c.h"

#define BME280_I2C_ADDR_0X76        (0x76U << 1)
#define BME280_I2C_ADDR_0X77        (0x77U << 1)
#define BME280_CHIP_ID_VALUE        (0x60U)

typedef enum
{
  BME280_OK = 0,
  BME280_ERROR,
  BME280_INVALID_PARAM,
  BME280_I2C_ERROR,
  BME280_TIMEOUT,
  BME280_NOT_FOUND,
  BME280_NOT_INITIALIZED,
  BME280_CALIB_ERROR,
  BME280_MEASUREMENT_BUSY
}
bme280_status_t;

typedef enum
{
  BME280_OVERSAMPLING_SKIPPED = 0,
  BME280_OVERSAMPLING_X1 = 1,
  BME280_OVERSAMPLING_X2 = 2,
  BME280_OVERSAMPLING_X4 = 3,
  BME280_OVERSAMPLING_X8 = 4,
  BME280_OVERSAMPLING_X16 = 5
}
bme280_oversampling_t;

typedef enum
{
  BME280_FILTER_OFF = 0,
  BME280_FILTER_2 = 1,
  BME280_FILTER_4 = 2,
  BME280_FILTER_8 = 3,
  BME280_FILTER_16 = 4
}
bme280_filter_t;

typedef enum
{
  BME280_STANDBY_0P5_MS = 0,
  BME280_STANDBY_62P5_MS,
  BME280_STANDBY_125_MS,
  BME280_STANDBY_250_MS,
  BME280_STANDBY_500_MS,
  BME280_STANDBY_1000_MS,
  BME280_STANDBY_10_MS,
  BME280_STANDBY_20_MS
}
bme280_standby_t;

typedef struct
{
  I2C_HandleTypeDef *hi2c;
  uint8_t i2c_addr;
  uint32_t timeout_ms;
}
bme280_config_t;

typedef struct
{
  uint16_t dig_T1;
  int16_t  dig_T2;
  int16_t  dig_T3;
  uint16_t dig_P1;
  int16_t  dig_P2;
  int16_t  dig_P3;
  int16_t  dig_P4;
  int16_t  dig_P5;
  int16_t  dig_P6;
  int16_t  dig_P7;
  int16_t  dig_P8;
  int16_t  dig_P9;
  uint8_t  dig_H1;
  int16_t  dig_H2;
  uint8_t  dig_H3;
  int16_t  dig_H4;
  int16_t  dig_H5;
  int8_t   dig_H6;
}
bme280_calib_data_t;

typedef struct
{
  bme280_oversampling_t osrs_t;
  bme280_oversampling_t osrs_p;
  bme280_oversampling_t osrs_h;
  bme280_filter_t       filter;
}
bme280_measurement_cfg_t;

typedef struct
{
  int32_t adc_temp;
  int32_t adc_press;
  int32_t adc_hum;
}
bme280_raw_data_t;

typedef struct
{
  int32_t  temperature_centi_c;
  uint32_t pressure_pa;
  uint32_t humidity_milli_pct;
}
bme280_data_t;

typedef struct
{
  bme280_config_t          config;
  bme280_calib_data_t      calib;
  bme280_measurement_cfg_t measurement_cfg;
  int32_t                  t_fine;
  uint8_t                  chip_id;
  uint8_t                  initialized;
}
bme280_handle_t;

bme280_status_t bme280_init(bme280_handle_t *handle, const bme280_config_t *config);
bme280_status_t bme280_soft_reset(bme280_handle_t *handle);
bme280_status_t bme280_read_chip_id(bme280_handle_t *handle, uint8_t *chip_id);
bme280_status_t bme280_configure(bme280_handle_t *handle, const bme280_measurement_cfg_t *cfg);
bme280_status_t bme280_trigger_forced_measurement(bme280_handle_t *handle);
bme280_status_t bme280_read_raw(bme280_handle_t *handle, bme280_raw_data_t *raw);
bme280_status_t bme280_read_compensated(bme280_handle_t *handle, bme280_data_t *data);
bme280_status_t bme280_is_measuring(bme280_handle_t *handle, uint8_t *is_busy);

#endif /* __BME280_H */

/* End of file -------------------------------------------------------- */
