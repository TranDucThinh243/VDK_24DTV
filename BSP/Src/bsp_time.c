/**
 * @file bsp_time.c
 * @brief BSP Time Module Source
 * @version 1.0.0
 */

#include "bsp_time.h"

/* Public functions */

uint32_t bsp_time_get_ms(void)
{
    return HAL_GetTick();
}

uint8_t bsp_time_is_elapsed(uint32_t start_tick, uint32_t interval_ms)
{
    uint32_t current_tick = bsp_time_get_ms();
    
    if ((current_tick - start_tick) >= interval_ms)
    {
        return 1;
    }
    
    return 0;
}

void bsp_time_delay_ms(uint32_t delay_ms)
{
    HAL_Delay(delay_ms);
}
