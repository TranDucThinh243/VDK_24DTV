/**
 * @file ssd1306.h
 * @brief SSD1306 OLED Device Driver Header
 * @version 1.0.0
 */

#ifndef SSD1306_H
#define SSD1306_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief SSD1306 font structure
 */
typedef struct {
    const uint8_t width;
    const uint8_t height;
    const uint16_t *const data;
    const uint8_t *const char_width;
} SSD1306_Font_t;

/**
 * @brief SSD1306 color enum
 */
typedef enum {
    SSD1306_COLOR_BLACK = 0x00,
    SSD1306_COLOR_WHITE = 0x01
} ssd1306_color_t;

/**
 * @brief SSD1306 status enum
 */
typedef enum {
    SSD1306_OK = 0,
    SSD1306_ERROR
} ssd1306_status_t;

/**
 * @brief Initialize the SSD1306 OLED display.
 * @return ssd1306_status_t Status of initialization.
 */
ssd1306_status_t ssd1306_init(void);

/**
 * @brief Clear the internal display buffer (does not update screen).
 */
void ssd1306_clear(void);

/**
 * @brief Send the buffer to the OLED screen via I2C.
 * @return ssd1306_status_t Status of the screen update.
 */
ssd1306_status_t ssd1306_update_screen(void);

/**
 * @brief Set the cursor position for drawing.
 * @param[in] x X coordinate (0-127).
 * @param[in] y Y coordinate (0-63).
 */
void ssd1306_set_cursor(uint8_t x, uint8_t y);

/**
 * @brief Write a string to the buffer at the current cursor position.
 * @param[in] str String to write.
 * @param[in] font Pointer to the font structure to use.
 * @param[in] color Color of the text.
 */
void ssd1306_write_string(const char *str, const SSD1306_Font_t *font, ssd1306_color_t color);

#ifdef __cplusplus
}
#endif

#endif /* SSD1306_H */
