/**
 * @file hc05.c
 * @brief HC-05 Bluetooth Device Driver Source
 * @version 1.0.0
 */

#include "hc05.h"
#include "bsp_uart.h"
#include <stddef.h>
#include <string.h>

/* Define a default timeout for HC-05 operations */
#define HC05_TIMEOUT_MS 100U

/* Assuming huart1 is initialized by CubeMX in main.c and used for HC-05 on PA9/PA10 */
extern UART_HandleTypeDef huart1;

/* Private function prototypes */
static hc05_status_t hc05_convert_status(bsp_uart_status_t bsp_status);

/* Public functions */

hc05_status_t hc05_init(void)
{
    /* Hardware UART initialization is handled by CubeMX.
       Any HC-05 specific AT command initialization could go here.
       For simple transparent data mode, nothing else is required. */
    return HC05_OK;
}

hc05_status_t hc05_send_bytes(uint8_t *p_data, uint16_t size)
{
    if (p_data == NULL || size == 0U)
    {
        return HC05_ERROR;
    }

    bsp_uart_status_t status = bsp_uart_transmit(&huart1, p_data, size, HC05_TIMEOUT_MS);
    return hc05_convert_status(status);
}

hc05_status_t hc05_send_string(const char *str)
{
    if (str == NULL)
    {
        return HC05_ERROR;
    }

    uint16_t len = (uint16_t)strlen(str);
    if (len == 0U)
    {
        return HC05_OK;
    }

    bsp_uart_status_t status = bsp_uart_transmit(&huart1, (uint8_t *)str, len, HC05_TIMEOUT_MS);
    return hc05_convert_status(status);
}

hc05_status_t hc05_receive_bytes(uint8_t *p_data, uint16_t size)
{
    if (p_data == NULL || size == 0U)
    {
        return HC05_ERROR;
    }

    bsp_uart_status_t status = bsp_uart_receive(&huart1, p_data, size, HC05_TIMEOUT_MS);
    return hc05_convert_status(status);
}

/* Private functions */

static hc05_status_t hc05_convert_status(bsp_uart_status_t bsp_status)
{
    switch (bsp_status)
    {
        case BSP_UART_OK:
            return HC05_OK;
        case BSP_UART_ERROR:
            return HC05_ERROR;
        case BSP_UART_BUSY:
            return HC05_BUSY;
        case BSP_UART_TIMEOUT:
            return HC05_TIMEOUT;
        default:
            return HC05_ERROR;
    }
}
