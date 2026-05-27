/**
 * @file bsp_delay.c
 * @brief BSP Delay Module Source
 */

#include "bsp_delay.h"

/* Public functions */

void bsp_delay_ms(uint32_t delay) {
    HAL_Delay(delay);
}

uint32_t bsp_delay_get_tick(void) {
    return HAL_GetTick();
}
