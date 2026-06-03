/**
 * @file display_service.h
 * @brief Display Service Module Header
 * @version 1.0.0
 */

#ifndef DISPLAY_SERVICE_H
#define DISPLAY_SERVICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "air_quality_service.h"

/**
 * @brief Display screens enum
 */
typedef enum {
    DISPLAY_SCREEN_SPLASH = 0,
    DISPLAY_SCREEN_HOME,
    DISPLAY_SCREEN_AIR,
    DISPLAY_SCREEN_BLUETOOTH,
    DISPLAY_SCREEN_ERROR
} display_screen_t;

/**
 * @brief Data structure passed from App to Display Service
 */
typedef struct {
    air_quality_data_t air_data;
    uint8_t bluetooth_active;
    uint8_t system_error;
} display_service_data_t;

/**
 * @brief Initialize the display service and underlying driver.
 */
void display_service_init(void);

/**
 * @brief Update the display with the provided data based on current screen.
 * @param[in] p_data Pointer to the display data structure.
 */
void display_service_update(const display_service_data_t *p_data);

/**
 * @brief Set the active display screen.
 * @param[in] screen The screen to switch to.
 */
void display_service_set_screen(display_screen_t screen);

/**
 * @brief Cycle to the next display screen.
 */
void display_service_next_screen(void);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_SERVICE_H */
