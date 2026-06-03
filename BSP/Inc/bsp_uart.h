/**
 * @file bsp_uart.h
 * @brief BSP UART Module Header
 * @version 1.0.0
 */

#ifndef BSP_UART_H
#define BSP_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"
#include <stdint.h>

/**
 * @brief UART Status enum
 */
typedef enum {
    BSP_UART_OK = 0,
    BSP_UART_ERROR,
    BSP_UART_BUSY,
    BSP_UART_TIMEOUT
} bsp_uart_status_t;

/**
 * @brief Transmit data over UART in blocking mode.
 * @param[in] huart UART handle pointer.
 * @param[in] p_data Pointer to data buffer to transmit.
 * @param[in] size Amount of data elements to be sent.
 * @param[in] timeout Timeout duration in milliseconds.
 * @return bsp_uart_status_t Status of the transmission.
 */
bsp_uart_status_t bsp_uart_transmit(UART_HandleTypeDef *huart, uint8_t *p_data, uint16_t size, uint32_t timeout);

/**
 * @brief Receive data over UART in blocking mode.
 * @param[in] huart UART handle pointer.
 * @param[out] p_data Pointer to data buffer to receive into.
 * @param[in] size Amount of data elements to be received.
 * @param[in] timeout Timeout duration in milliseconds.
 * @return bsp_uart_status_t Status of the reception.
 */
bsp_uart_status_t bsp_uart_receive(UART_HandleTypeDef *huart, uint8_t *p_data, uint16_t size, uint32_t timeout);

/**
 * @brief Transmit data over UART in non-blocking mode with Interrupt.
 * @param[in] huart UART handle pointer.
 * @param[in] p_data Pointer to data buffer to transmit.
 * @param[in] size Amount of data elements to be sent.
 * @return bsp_uart_status_t Status of the transmission initiation.
 */
bsp_uart_status_t bsp_uart_transmit_it(UART_HandleTypeDef *huart, uint8_t *p_data, uint16_t size);

/**
 * @brief Receive data over UART in non-blocking mode with Interrupt.
 * @param[in] huart UART handle pointer.
 * @param[out] p_data Pointer to data buffer to receive into.
 * @param[in] size Amount of data elements to be received.
 * @return bsp_uart_status_t Status of the reception initiation.
 */
bsp_uart_status_t bsp_uart_receive_it(UART_HandleTypeDef *huart, uint8_t *p_data, uint16_t size);

#ifdef __cplusplus
}
#endif

#endif /* BSP_UART_H */
