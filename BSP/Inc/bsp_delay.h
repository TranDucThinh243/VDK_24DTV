/**
 * @file bsp_delay.h
 * @brief BSP Delay Module Header
 */

#ifndef BSP_DELAY_H
#define BSP_DELAY_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

void bsp_delay_ms(uint32_t delay);
uint32_t bsp_delay_get_tick(void);

#endif /* BSP_DELAY_H */
