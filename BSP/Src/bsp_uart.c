/**
 * @file bsp_uart.c
 * @brief BSP UART Module Source
 */

#include "bsp_uart.h"
#include <stddef.h>

/* Private function prototypes */
static bsp_uart_status_t bsp_uart_convert_status(HAL_StatusTypeDef hal_status);

/* Public functions */

bsp_uart_status_t bsp_uart_transmit(UART_HandleTypeDef *huart, uint8_t *p_data, uint16_t size, uint32_t timeout) {
    if (huart == NULL || p_data == NULL || size == 0) {
        return BSP_UART_ERROR;
    }
    HAL_StatusTypeDef status = HAL_UART_Transmit(huart, p_data, size, timeout);
    return bsp_uart_convert_status(status);
}

bsp_uart_status_t bsp_uart_receive(UART_HandleTypeDef *huart, uint8_t *p_data, uint16_t size, uint32_t timeout) {
    if (huart == NULL || p_data == NULL || size == 0) {
        return BSP_UART_ERROR;
    }
    HAL_StatusTypeDef status = HAL_UART_Receive(huart, p_data, size, timeout);
    return bsp_uart_convert_status(status);
}

bsp_uart_status_t bsp_uart_transmit_it(UART_HandleTypeDef *huart, uint8_t *p_data, uint16_t size) {
    if (huart == NULL || p_data == NULL || size == 0) {
        return BSP_UART_ERROR;
    }
    HAL_StatusTypeDef status = HAL_UART_Transmit_IT(huart, p_data, size);
    return bsp_uart_convert_status(status);
}

bsp_uart_status_t bsp_uart_receive_it(UART_HandleTypeDef *huart, uint8_t *p_data, uint16_t size) {
    if (huart == NULL || p_data == NULL || size == 0) {
        return BSP_UART_ERROR;
    }
    HAL_StatusTypeDef status = HAL_UART_Receive_IT(huart, p_data, size);
    return bsp_uart_convert_status(status);
}

/* Private functions */

static bsp_uart_status_t bsp_uart_convert_status(HAL_StatusTypeDef hal_status) {
    switch (hal_status) {
        case HAL_OK:
            return BSP_UART_OK;
        case HAL_ERROR:
            return BSP_UART_ERROR;
        case HAL_BUSY:
            return BSP_UART_BUSY;
        case HAL_TIMEOUT:
            return BSP_UART_TIMEOUT;
        default:
            return BSP_UART_ERROR;
    }
}
