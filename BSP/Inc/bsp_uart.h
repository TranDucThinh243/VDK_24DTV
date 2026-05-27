/**
 * @file bsp_uart.h
 * @brief BSP UART Module Header
 */

#ifndef BSP_UART_H
#define BSP_UART_H

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/**
 * @brief UART Status enum
 */
typedef enum {
    BSP_UART_OK = 0,
    BSP_UART_ERROR,
    BSP_UART_BUSY,
    BSP_UART_TIMEOUT
} bsp_uart_status_t;

bsp_uart_status_t bsp_uart_transmit(UART_HandleTypeDef *huart, uint8_t *p_data, uint16_t size, uint32_t timeout);
bsp_uart_status_t bsp_uart_receive(UART_HandleTypeDef *huart, uint8_t *p_data, uint16_t size, uint32_t timeout);
bsp_uart_status_t bsp_uart_transmit_it(UART_HandleTypeDef *huart, uint8_t *p_data, uint16_t size);
bsp_uart_status_t bsp_uart_receive_it(UART_HandleTypeDef *huart, uint8_t *p_data, uint16_t size);

#endif /* BSP_UART_H */
