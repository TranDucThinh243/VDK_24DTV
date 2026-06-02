/**
 * @file App.h
 * @brief Core Application Interface for HC-05 & SSD1306 OLED features
 */

#ifndef APP_H
#define APP_H

#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

/* Public function prototypes */
void App_Init(void);
void App_Loop(void);

#endif /* APP_H */
