/**
 * @file ssd1306.c
 * @brief SSD1306 OLED Device Driver Source
 * @version 1.0.0
 */

#include "ssd1306.h"
#include "bsp_i2c.h"
#include "bsp_time.h"
#include <string.h>

#define SSD1306_I2C_ADDR         (0x3C << 1)
#define SSD1306_WIDTH            128
#define SSD1306_HEIGHT           64
#define SSD1306_BUFFER_SIZE      (SSD1306_WIDTH * SSD1306_HEIGHT / 8)
#define SSD1306_TIMEOUT_MS       100

extern I2C_HandleTypeDef hi2c1;

/* Private variables */
static uint8_t s_oled_buffer[SSD1306_BUFFER_SIZE];
static uint8_t s_cursor_x = 0;
static uint8_t s_cursor_y = 0;

/* Private function prototypes */
static ssd1306_status_t ssd1306_write_command(uint8_t command);
static void ssd1306_draw_pixel(uint8_t x, uint8_t y, ssd1306_color_t color);
static char ssd1306_write_char(char ch, const SSD1306_Font_t *font, ssd1306_color_t color);

/* Public functions */

ssd1306_status_t ssd1306_init(void)
{
    bsp_time_delay_ms(100);

    /* Display Off */
    if (ssd1306_write_command(0xAE) != SSD1306_OK) return SSD1306_ERROR;
    
    /* Setup display sequence */
    ssd1306_write_command(0xD5);
    ssd1306_write_command(0x80);
    ssd1306_write_command(0xA8);
    ssd1306_write_command(0x3F);
    ssd1306_write_command(0xD3);
    ssd1306_write_command(0x00);
    ssd1306_write_command(0x40);
    ssd1306_write_command(0x8D);
    ssd1306_write_command(0x14);
    ssd1306_write_command(0x20);
    ssd1306_write_command(0x00);
    ssd1306_write_command(0xA1);
    ssd1306_write_command(0xC8);
    ssd1306_write_command(0xDA);
    ssd1306_write_command(0x12);
    ssd1306_write_command(0x81);
    ssd1306_write_command(0x7F);
    ssd1306_write_command(0xD9);
    ssd1306_write_command(0xF1);
    ssd1306_write_command(0xDB);
    ssd1306_write_command(0x40);
    ssd1306_write_command(0xA4);
    ssd1306_write_command(0xA6);
    
    ssd1306_clear();
    
    if (ssd1306_update_screen() != SSD1306_OK)
    {
        return SSD1306_ERROR;
    }
    
    /* Display On */
    if (ssd1306_write_command(0xAF) != SSD1306_OK)
    {
        return SSD1306_ERROR;
    }
    
    return SSD1306_OK;
}

void ssd1306_clear(void)
{
    memset(s_oled_buffer, 0x00, sizeof(s_oled_buffer));
}

ssd1306_status_t ssd1306_update_screen(void)
{
    uint8_t i;
    for (i = 0; i < 8; i++)
    {
        ssd1306_write_command(0xB0 + i);
        ssd1306_write_command(0x00);
        ssd1306_write_command(0x10);
        
        bsp_i2c_status_t status = bsp_i2c_mem_write(&hi2c1, SSD1306_I2C_ADDR, 0x40, 1, &s_oled_buffer[SSD1306_WIDTH * i], SSD1306_WIDTH, SSD1306_TIMEOUT_MS);
        if (status != BSP_I2C_OK)
        {
            return SSD1306_ERROR;
        }
    }
    return SSD1306_OK;
}

void ssd1306_set_cursor(uint8_t x, uint8_t y)
{
    s_cursor_x = x;
    s_cursor_y = y;
}

void ssd1306_write_string(const char *str, const SSD1306_Font_t *font, ssd1306_color_t color)
{
    if (str == NULL || font == NULL) return;
    
    while (*str)
    {
        if (ssd1306_write_char(*str, font, color) != *str)
        {
            break;
        }
        str++;
    }
}

/* Private functions */

static ssd1306_status_t ssd1306_write_command(uint8_t command)
{
    bsp_i2c_status_t status = bsp_i2c_mem_write(&hi2c1, SSD1306_I2C_ADDR, 0x00, 1, &command, 1, SSD1306_TIMEOUT_MS);
    if (status != BSP_I2C_OK)
    {
        return SSD1306_ERROR;
    }
    return SSD1306_OK;
}

static void ssd1306_draw_pixel(uint8_t x, uint8_t y, ssd1306_color_t color)
{
    if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) return;

    if (color == SSD1306_COLOR_WHITE)
    {
        s_oled_buffer[x + (y / 8) * SSD1306_WIDTH] |= (1 << (y % 8));
    }
    else
    {
        s_oled_buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
    }
}

static char ssd1306_write_char(char ch, const SSD1306_Font_t *font, ssd1306_color_t color)
{
    uint32_t i, b, j;

    if (ch < 32 || ch > 126) return 0;
    if (SSD1306_WIDTH < (s_cursor_x + font->width) || SSD1306_HEIGHT < (s_cursor_y + font->height)) return 0;

    for (i = 0; i < font->height; i++)
    {
        b = font->data[(ch - 32) * font->height + i];
        for (j = 0; j < font->width; j++)
        {
            if ((b << j) & 0x8000)
            {
                ssd1306_draw_pixel(s_cursor_x + j, (s_cursor_y + i), color);
            }
            else
            {
                ssd1306_draw_pixel(s_cursor_x + j, (s_cursor_y + i), (ssd1306_color_t)!color);
            }
        }
    }
    
    s_cursor_x += font->width;
    return ch;
}
