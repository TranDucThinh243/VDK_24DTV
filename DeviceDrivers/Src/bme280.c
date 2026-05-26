/**
 * @file       bme280.c
 * @version    1.0.0
 * @date       2026-05-22
 * @author     Tran Duc Thinh
 * @brief      Portable BME280 I2C driver implementation
 */

#include "bme280.h"
#include <string.h>

#define BME280_REG_CHIP_ID          (0xD0U)
#define BME280_REG_RESET            (0xE0U)
#define BME280_REG_STATUS           (0xF3U)
#define BME280_REG_CTRL_HUM         (0xF2U)
#define BME280_REG_CTRL_MEAS        (0xF4U)
#define BME280_REG_CONFIG           (0xF5U)
#define BME280_REG_PRESS_MSB        (0xF7U)
#define BME280_REG_CALIB00          (0x88U)
#define BME280_REG_CALIB26          (0xE1U)

#define BME280_RESET_VALUE          (0xB6U)
#define BME280_REG_ADDR_SIZE        (1U)

#define BME280_MODE_SLEEP           (0x00U)
#define BME280_MODE_FORCED          (0x01U)

#define BME280_STATUS_IM_UPDATE     (0x01U)
#define BME280_STATUS_MEASURING     (0x08U)

#define BME280_RESET_DELAY_MS       (2U)

static bme280_status_t bme280_from_bsp_status(bsp_i2c_status_t status);
static bme280_status_t bme280_validate_config(const bme280_config_t *config);
static bme280_status_t bme280_validate_measurement_cfg(const bme280_measurement_cfg_t *cfg);
static bme280_status_t bme280_read_bytes(bme280_handle_t *handle, uint8_t reg_addr, uint8_t *buffer, uint16_t size);
static bme280_status_t bme280_write_bytes(bme280_handle_t *handle, uint8_t reg_addr, const uint8_t *buffer, uint16_t size);
static bme280_status_t bme280_read_status_reg(bme280_handle_t *handle, uint8_t *status_reg);
static bme280_status_t bme280_wait_status_clear(bme280_handle_t *handle, uint8_t mask);
static bme280_status_t bme280_load_calibration(bme280_handle_t *handle);
static bme280_status_t bme280_read_raw_registers(bme280_handle_t *handle, bme280_raw_data_t *raw);
static int32_t bme280_compensate_temperature(bme280_handle_t *handle, int32_t adc_temp);
static bme280_status_t bme280_compensate_pressure(bme280_handle_t *handle, int32_t adc_press, uint32_t *pressure_pa);
static uint32_t bme280_compensate_humidity(bme280_handle_t *handle, int32_t adc_hum);
static uint16_t bme280_u16_le(const uint8_t *buffer);
static int16_t bme280_s16_le(const uint8_t *buffer);

bme280_status_t bme280_init(bme280_handle_t *handle, const bme280_config_t *config)
{
  bme280_measurement_cfg_t default_cfg;
  bme280_status_t status;

  if ((handle == NULL) || (config == NULL))
  {
    return BME280_INVALID_PARAM;
  }

  status = bme280_validate_config(config);
  if (status != BME280_OK)
  {
    return status;
  }

  memset(handle, 0, sizeof(*handle));
  handle->config = *config;

  status = bme280_read_chip_id(handle, &handle->chip_id);
  if (status != BME280_OK)
  {
    return status;
  }

  if (handle->chip_id != BME280_CHIP_ID_VALUE)
  {
    return BME280_NOT_FOUND;
  }

  status = bme280_soft_reset(handle);
  if (status != BME280_OK)
  {
    return status;
  }

  status = bme280_load_calibration(handle);
  if (status != BME280_OK)
  {
    return status;
  }

  default_cfg.osrs_t = BME280_OVERSAMPLING_X1;
  default_cfg.osrs_p = BME280_OVERSAMPLING_X1;
  default_cfg.osrs_h = BME280_OVERSAMPLING_X1;
  default_cfg.filter = BME280_FILTER_OFF;

  status = bme280_configure(handle, &default_cfg);
  if (status != BME280_OK)
  {
    return status;
  }

  handle->initialized = 1U;

  return BME280_OK;
}

bme280_status_t bme280_soft_reset(bme280_handle_t *handle)
{
  uint8_t reset_cmd = BME280_RESET_VALUE;
  bme280_status_t status;

  if (handle == NULL)
  {
    return BME280_INVALID_PARAM;
  }

  status = bme280_write_bytes(handle, BME280_REG_RESET, &reset_cmd, 1U);
  if (status != BME280_OK)
  {
    return status;
  }

  bsp_i2c_delay_ms(BME280_RESET_DELAY_MS);

  return bme280_wait_status_clear(handle, BME280_STATUS_IM_UPDATE | BME280_STATUS_MEASURING);
}

bme280_status_t bme280_read_chip_id(bme280_handle_t *handle, uint8_t *chip_id)
{
  if ((handle == NULL) || (chip_id == NULL))
  {
    return BME280_INVALID_PARAM;
  }

  return bme280_read_bytes(handle, BME280_REG_CHIP_ID, chip_id, 1U);
}

bme280_status_t bme280_configure(bme280_handle_t *handle, const bme280_measurement_cfg_t *cfg)
{
  uint8_t ctrl_hum;
  uint8_t config_reg;
  uint8_t ctrl_meas;
  bme280_status_t status;

  if ((handle == NULL) || (cfg == NULL))
  {
    return BME280_INVALID_PARAM;
  }

  status = bme280_validate_measurement_cfg(cfg);
  if (status != BME280_OK)
  {
    return status;
  }

  ctrl_hum = (uint8_t)cfg->osrs_h;
  config_reg = (uint8_t)(cfg->filter << 2);
  ctrl_meas = (uint8_t)((cfg->osrs_t << 5) | (cfg->osrs_p << 2) | BME280_MODE_FORCED);

  status = bme280_write_bytes(handle, BME280_REG_CTRL_HUM, &ctrl_hum, 1U);
  if (status != BME280_OK)
  {
    return status;
  }

  status = bme280_write_bytes(handle, BME280_REG_CONFIG, &config_reg, 1U);
  if (status != BME280_OK)
  {
    return status;
  }

  status = bme280_write_bytes(handle, BME280_REG_CTRL_MEAS, &ctrl_meas, 1U);
  if (status != BME280_OK)
  {
    return status;
  }

  handle->measurement_cfg = *cfg;

  return BME280_OK;
}

bme280_status_t bme280_trigger_forced_measurement(bme280_handle_t *handle)
{
  uint8_t ctrl_meas;

  if (handle == NULL)
  {
    return BME280_INVALID_PARAM;
  }

  if (handle->initialized == 0U)
  {
    return BME280_NOT_INITIALIZED;
  }

  ctrl_meas = (uint8_t)((handle->measurement_cfg.osrs_t << 5)
                      | (handle->measurement_cfg.osrs_p << 2)
                      | BME280_MODE_FORCED);

  return bme280_write_bytes(handle, BME280_REG_CTRL_MEAS, &ctrl_meas, 1U);
}

bme280_status_t bme280_read_raw(bme280_handle_t *handle, bme280_raw_data_t *raw)
{
  bme280_status_t status;

  if ((handle == NULL) || (raw == NULL))
  {
    return BME280_INVALID_PARAM;
  }

  if (handle->initialized == 0U)
  {
    return BME280_NOT_INITIALIZED;
  }

  status = bme280_trigger_forced_measurement(handle);
  if (status != BME280_OK)
  {
    return status;
  }

  status = bme280_wait_status_clear(handle, BME280_STATUS_MEASURING);
  if (status != BME280_OK)
  {
    return status;
  }

  return bme280_read_raw_registers(handle, raw);
}

bme280_status_t bme280_read_compensated(bme280_handle_t *handle, bme280_data_t *data)
{
  bme280_raw_data_t raw;
  bme280_status_t status;

  if ((handle == NULL) || (data == NULL))
  {
    return BME280_INVALID_PARAM;
  }

  status = bme280_read_raw(handle, &raw);
  if (status != BME280_OK)
  {
    return status;
  }

  data->temperature_centi_c = bme280_compensate_temperature(handle, raw.adc_temp);

  status = bme280_compensate_pressure(handle, raw.adc_press, &data->pressure_pa);
  if (status != BME280_OK)
  {
    return status;
  }

  data->humidity_milli_pct = bme280_compensate_humidity(handle, raw.adc_hum);

  return BME280_OK;
}

bme280_status_t bme280_is_measuring(bme280_handle_t *handle, uint8_t *is_busy)
{
  uint8_t status_reg = 0U;
  bme280_status_t status;

  if ((handle == NULL) || (is_busy == NULL))
  {
    return BME280_INVALID_PARAM;
  }

  if (handle->initialized == 0U)
  {
    return BME280_NOT_INITIALIZED;
  }

  status = bme280_read_status_reg(handle, &status_reg);
  if (status != BME280_OK)
  {
    return status;
  }

  *is_busy = ((status_reg & BME280_STATUS_MEASURING) != 0U) ? 1U : 0U;

  return BME280_OK;
}

static bme280_status_t bme280_from_bsp_status(bsp_i2c_status_t status)
{
  switch (status)
  {
    case BSP_I2C_OK:
      return BME280_OK;
    case BSP_I2C_TIMEOUT:
      return BME280_TIMEOUT;
    case BSP_I2C_BUSY:
    case BSP_I2C_ERROR:
    default:
      return BME280_I2C_ERROR;
  }
}

static bme280_status_t bme280_validate_config(const bme280_config_t *config)
{
  if ((config == NULL)
   || (config->hi2c == NULL)
   || (config->timeout_ms == 0U)
  )
  {
    return BME280_INVALID_PARAM;
  }

  if ((config->i2c_addr != BME280_I2C_ADDR_0X76)
   && (config->i2c_addr != BME280_I2C_ADDR_0X77))
  {
    return BME280_INVALID_PARAM;
  }

  return BME280_OK;
}

static bme280_status_t bme280_validate_measurement_cfg(const bme280_measurement_cfg_t *cfg)
{
  if (cfg == NULL)
  {
    return BME280_INVALID_PARAM;
  }

  if ((cfg->osrs_t < BME280_OVERSAMPLING_X1)
   || (cfg->osrs_t > BME280_OVERSAMPLING_X16)
   || (cfg->osrs_p < BME280_OVERSAMPLING_X1)
   || (cfg->osrs_p > BME280_OVERSAMPLING_X16)
   || (cfg->osrs_h < BME280_OVERSAMPLING_X1)
   || (cfg->osrs_h > BME280_OVERSAMPLING_X16)
   || (cfg->filter > BME280_FILTER_16))
  {
    return BME280_INVALID_PARAM;
  }

  return BME280_OK;
}

static bme280_status_t bme280_read_bytes(bme280_handle_t *handle, uint8_t reg_addr, uint8_t *buffer, uint16_t size)
{
  if ((handle == NULL) || (handle->config.hi2c == NULL) || (buffer == NULL) || (size == 0U))
  {
    return BME280_INVALID_PARAM;
  }

  return bme280_from_bsp_status(bsp_i2c_mem_read(handle->config.hi2c,
                                                 handle->config.i2c_addr,
                                                 reg_addr,
                                                 BME280_REG_ADDR_SIZE,
                                                 buffer,
                                                 size,
                                                 handle->config.timeout_ms));
}

static bme280_status_t bme280_write_bytes(bme280_handle_t *handle, uint8_t reg_addr, const uint8_t *buffer, uint16_t size)
{
  if ((handle == NULL) || (handle->config.hi2c == NULL) || (buffer == NULL) || (size == 0U))
  {
    return BME280_INVALID_PARAM;
  }

  return bme280_from_bsp_status(bsp_i2c_mem_write(handle->config.hi2c,
                                                  handle->config.i2c_addr,
                                                  reg_addr,
                                                  BME280_REG_ADDR_SIZE,
                                                  buffer,
                                                  size,
                                                  handle->config.timeout_ms));
}

static bme280_status_t bme280_read_status_reg(bme280_handle_t *handle, uint8_t *status_reg)
{
  return bme280_read_bytes(handle, BME280_REG_STATUS, status_reg, 1U);
}

static bme280_status_t bme280_wait_status_clear(bme280_handle_t *handle, uint8_t mask)
{
  uint8_t status_reg = 0U;
  uint32_t start_tick;
  bme280_status_t status;

  if (handle == NULL)
  {
    return BME280_INVALID_PARAM;
  }

  start_tick = bsp_i2c_get_tick();

  do
  {
    status = bme280_read_status_reg(handle, &status_reg);
    if (status != BME280_OK)
    {
      return status;
    }

    if ((status_reg & mask) == 0U)
    {
      return BME280_OK;
    }
  }
  while ((bsp_i2c_get_tick() - start_tick) < handle->config.timeout_ms);

  return BME280_TIMEOUT;
}

static bme280_status_t bme280_load_calibration(bme280_handle_t *handle)
{
  uint8_t calib1[26];
  uint8_t calib2[7];
  bme280_status_t status;

  if (handle == NULL)
  {
    return BME280_INVALID_PARAM;
  }

  status = bme280_read_bytes(handle, BME280_REG_CALIB00, calib1, sizeof(calib1));
  if (status != BME280_OK)
  {
    return status;
  }

  status = bme280_read_bytes(handle, BME280_REG_CALIB26, calib2, sizeof(calib2));
  if (status != BME280_OK)
  {
    return status;
  }

  handle->calib.dig_T1 = bme280_u16_le(&calib1[0]);
  handle->calib.dig_T2 = bme280_s16_le(&calib1[2]);
  handle->calib.dig_T3 = bme280_s16_le(&calib1[4]);
  handle->calib.dig_P1 = bme280_u16_le(&calib1[6]);
  handle->calib.dig_P2 = bme280_s16_le(&calib1[8]);
  handle->calib.dig_P3 = bme280_s16_le(&calib1[10]);
  handle->calib.dig_P4 = bme280_s16_le(&calib1[12]);
  handle->calib.dig_P5 = bme280_s16_le(&calib1[14]);
  handle->calib.dig_P6 = bme280_s16_le(&calib1[16]);
  handle->calib.dig_P7 = bme280_s16_le(&calib1[18]);
  handle->calib.dig_P8 = bme280_s16_le(&calib1[20]);
  handle->calib.dig_P9 = bme280_s16_le(&calib1[22]);
  handle->calib.dig_H1 = calib1[25];
  handle->calib.dig_H2 = bme280_s16_le(&calib2[0]);
  handle->calib.dig_H3 = calib2[2];
  handle->calib.dig_H4 = (int16_t)(((int16_t)((int8_t)calib2[3]) << 4) | (calib2[4] & 0x0F));
  handle->calib.dig_H5 = (int16_t)(((int16_t)((int8_t)calib2[5]) << 4) | (calib2[4] >> 4));
  handle->calib.dig_H6 = (int8_t)calib2[6];

  if (handle->calib.dig_P1 == 0U)
  {
    return BME280_CALIB_ERROR;
  }

  return BME280_OK;
}

static bme280_status_t bme280_read_raw_registers(bme280_handle_t *handle, bme280_raw_data_t *raw)
{
  uint8_t buffer[8];
  bme280_status_t status;

  status = bme280_read_bytes(handle, BME280_REG_PRESS_MSB, buffer, sizeof(buffer));
  if (status != BME280_OK)
  {
    return status;
  }

  raw->adc_press = (int32_t)(((uint32_t)buffer[0] << 12)
                           | ((uint32_t)buffer[1] << 4)
                           | ((uint32_t)buffer[2] >> 4));
  raw->adc_temp = (int32_t)(((uint32_t)buffer[3] << 12)
                          | ((uint32_t)buffer[4] << 4)
                          | ((uint32_t)buffer[5] >> 4));
  raw->adc_hum = (int32_t)(((uint32_t)buffer[6] << 8) | buffer[7]);

  return BME280_OK;
}

static int32_t bme280_compensate_temperature(bme280_handle_t *handle, int32_t adc_temp)
{
  int32_t var1;
  int32_t var2;

  var1 = ((((adc_temp >> 3) - ((int32_t)handle->calib.dig_T1 << 1))) * ((int32_t)handle->calib.dig_T2)) >> 11;
  var2 = (((((adc_temp >> 4) - ((int32_t)handle->calib.dig_T1))
         * ((adc_temp >> 4) - ((int32_t)handle->calib.dig_T1))) >> 12)
         * ((int32_t)handle->calib.dig_T3)) >> 14;

  handle->t_fine = var1 + var2;

  return (handle->t_fine * 5 + 128) >> 8;
}

static bme280_status_t bme280_compensate_pressure(bme280_handle_t *handle, int32_t adc_press, uint32_t *pressure_pa)
{
  int64_t var1;
  int64_t var2;
  int64_t pressure_q24_8;

  if ((handle == NULL) || (pressure_pa == NULL))
  {
    return BME280_INVALID_PARAM;
  }

  var1 = ((int64_t)handle->t_fine) - 128000LL;
  var2 = var1 * var1 * (int64_t)handle->calib.dig_P6;
  var2 = var2 + ((var1 * (int64_t)handle->calib.dig_P5) << 17);
  var2 = var2 + (((int64_t)handle->calib.dig_P4) << 35);
  var1 = ((var1 * var1 * (int64_t)handle->calib.dig_P3) >> 8)
       + ((var1 * (int64_t)handle->calib.dig_P2) << 12);
  var1 = ((((int64_t)1) << 47) + var1) * (int64_t)handle->calib.dig_P1 >> 33;

  if (var1 == 0LL)
  {
    return BME280_CALIB_ERROR;
  }

  pressure_q24_8 = 1048576LL - adc_press;
  pressure_q24_8 = (((pressure_q24_8 << 31) - var2) * 3125LL) / var1;
  var1 = (((int64_t)handle->calib.dig_P9) * (pressure_q24_8 >> 13) * (pressure_q24_8 >> 13)) >> 25;
  var2 = (((int64_t)handle->calib.dig_P8) * pressure_q24_8) >> 19;
  pressure_q24_8 = ((pressure_q24_8 + var1 + var2) >> 8) + (((int64_t)handle->calib.dig_P7) << 4);

  *pressure_pa = (uint32_t)(pressure_q24_8 >> 8);

  return BME280_OK;
}

static uint32_t bme280_compensate_humidity(bme280_handle_t *handle, int32_t adc_hum)
{
  int32_t v_x1_u32r;
  uint32_t humidity_q22_10;
  uint32_t humidity_milli_pct;

  v_x1_u32r = handle->t_fine - 76800;
  v_x1_u32r = (((((adc_hum << 14)
                - (((int32_t)handle->calib.dig_H4) << 20)
                - (((int32_t)handle->calib.dig_H5) * v_x1_u32r))
                + 16384) >> 15)
                * (((((((v_x1_u32r * ((int32_t)handle->calib.dig_H6)) >> 10)
                * (((v_x1_u32r * ((int32_t)handle->calib.dig_H3)) >> 11) + 32768)) >> 10)
                + 2097152) * ((int32_t)handle->calib.dig_H2) + 8192) >> 14));
  v_x1_u32r = v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7)
              * ((int32_t)handle->calib.dig_H1)) >> 4);
  v_x1_u32r = (v_x1_u32r < 0) ? 0 : v_x1_u32r;
  v_x1_u32r = (v_x1_u32r > 419430400) ? 419430400 : v_x1_u32r;

  humidity_q22_10 = (uint32_t)(v_x1_u32r >> 12);
  humidity_milli_pct = (uint32_t)((((uint64_t)humidity_q22_10) * 1000ULL + 512ULL) / 1024ULL);

  if (humidity_milli_pct > 100000UL)
  {
    humidity_milli_pct = 100000UL;
  }

  return humidity_milli_pct;
}

static uint16_t bme280_u16_le(const uint8_t *buffer)
{
  return (uint16_t)(((uint16_t)buffer[1] << 8) | buffer[0]);
}

static int16_t bme280_s16_le(const uint8_t *buffer)
{
  return (int16_t)(((uint16_t)buffer[1] << 8) | buffer[0]);
}

/* End of file -------------------------------------------------------- */
