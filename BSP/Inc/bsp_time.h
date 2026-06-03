/**
 * @file bsp_time.h
 * @brief BSP Time Module Header
 * @version 1.0.0
 */

#ifndef BSP_TIME_H
#define BSP_TIME_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <stdint.h>

/**
 * @brief Get the current system tick in milliseconds.
 * @return uint32_t Current tick count.
 */
uint32_t bsp_time_get_ms(void);

/**
 * @brief Check if a specific time interval has elapsed (handles overflow).
 * @param[in] start_tick The starting tick value.
 * @param[in] interval_ms The interval to check against in milliseconds.
 * @return uint8_t 1 if the interval has elapsed, 0 otherwise.
 */
uint8_t bsp_time_is_elapsed(uint32_t start_tick, uint32_t interval_ms);

/**
 * @brief Block execution for a specified number of milliseconds.
 * @param[in] delay_ms Number of milliseconds to delay.
 */
void bsp_time_delay_ms(uint32_t delay_ms);

#ifdef __cplusplus
}
#endif

#endif /* BSP_TIME_H */
