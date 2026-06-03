/**
 * @file display_service.c
 * @brief Display Service Module Source
 * @version 1.0.0
 */

#include "display_service.h"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include <stddef.h>
#include <stdio.h>

/* Static variables */
static display_screen_t s_current_screen = DISPLAY_SCREEN_SPLASH;

/* Private function prototypes */
static void draw_splash_screen(void);
static void draw_home_screen(const display_service_data_t *p_data);
static void draw_air_screen(const display_service_data_t *p_data);
static void draw_bluetooth_screen(const display_service_data_t *p_data);
static void draw_error_screen(void);

/* Public functions */

void display_service_init(void)
{
    ssd1306_init();
    s_current_screen = DISPLAY_SCREEN_SPLASH;
}

void display_service_update(const display_service_data_t *p_data)
{
    if (p_data == NULL)
    {
        /* If no data provided, display error screen temporarily */
        ssd1306_clear();
        ssd1306_set_cursor(10, 20);
        ssd1306_write_string("DATA NULL", &Font_7x10, SSD1306_COLOR_WHITE);
        ssd1306_update_screen();
        return;
    }

    ssd1306_clear();

    switch (s_current_screen)
    {
        case DISPLAY_SCREEN_SPLASH:
            draw_splash_screen();
            break;
        case DISPLAY_SCREEN_HOME:
            draw_home_screen(p_data);
            break;
        case DISPLAY_SCREEN_AIR:
            draw_air_screen(p_data);
            break;
        case DISPLAY_SCREEN_BLUETOOTH:
            draw_bluetooth_screen(p_data);
            break;
        case DISPLAY_SCREEN_ERROR:
            draw_error_screen();
            break;
        default:
            draw_error_screen();
            break;
    }

    ssd1306_update_screen();
}

void display_service_set_screen(display_screen_t screen)
{
    if (screen <= DISPLAY_SCREEN_ERROR)
    {
        s_current_screen = screen;
    }
}

void display_service_next_screen(void)
{
    /* Cycle through screens (skip splash and error normally) */
    if (s_current_screen == DISPLAY_SCREEN_SPLASH || s_current_screen == DISPLAY_SCREEN_ERROR)
    {
        s_current_screen = DISPLAY_SCREEN_HOME;
    }
    else
    {
        s_current_screen++;
        if (s_current_screen > DISPLAY_SCREEN_BLUETOOTH)
        {
            s_current_screen = DISPLAY_SCREEN_HOME;
        }
    }
}

/* Private functions */

static void draw_splash_screen(void)
{
    ssd1306_set_cursor(20, 25);
    ssd1306_write_string("STM32 SYS", &Font_11x18, SSD1306_COLOR_WHITE);
}

static void draw_home_screen(const display_service_data_t *p_data)
{
    ssd1306_set_cursor(5, 5);
    ssd1306_write_string("HOME DASHBOARD", &Font_7x10, SSD1306_COLOR_WHITE);

    ssd1306_set_cursor(5, 25);
    if (p_data->system_error)
    {
        ssd1306_write_string("SYS: ERROR", &Font_7x10, SSD1306_COLOR_WHITE);
    }
    else
    {
        ssd1306_write_string("SYS: OK", &Font_7x10, SSD1306_COLOR_WHITE);
    }

    ssd1306_set_cursor(5, 45);
    if (p_data->bluetooth_active)
    {
        ssd1306_write_string("BT: ACTIVE", &Font_7x10, SSD1306_COLOR_WHITE);
    }
    else
    {
        ssd1306_write_string("BT: IDLE", &Font_7x10, SSD1306_COLOR_WHITE);
    }
}

static void draw_air_screen(const display_service_data_t *p_data)
{
    char buf[32];

    ssd1306_set_cursor(5, 5);
    ssd1306_write_string("AIR QUALITY", &Font_7x10, SSD1306_COLOR_WHITE);

    if (!p_data->air_data.sensor_ok)
    {
        ssd1306_set_cursor(5, 25);
        ssd1306_write_string("SENSOR ERROR", &Font_7x10, SSD1306_COLOR_WHITE);
        return;
    }

    const char *sts_str = "UNKNOWN";
    switch (p_data->air_data.level)
    {
        case AIR_QUALITY_GOOD:    sts_str = "NORMAL"; break;
        case AIR_QUALITY_WARNING: sts_str = "WARNING"; break;
        case AIR_QUALITY_DANGER:  sts_str = "DANGER"; break;
    }

    /* Line 2: Status */
    snprintf(buf, sizeof(buf), "Sts: %s", sts_str);
    ssd1306_set_cursor(5, 17);
    ssd1306_write_string(buf, &Font_7x10, SSD1306_COLOR_WHITE);

    /* Line 3: PPM */
    snprintf(buf, sizeof(buf), "PPM: %lu", (unsigned long)p_data->air_data.ppm_est);
    ssd1306_set_cursor(5, 29);
    ssd1306_write_string(buf, &Font_7x10, SSD1306_COLOR_WHITE);

    /* Line 4: Delta */
    snprintf(buf, sizeof(buf), "Dlt: +%lu", (unsigned long)p_data->air_data.delta_ppm);
    ssd1306_set_cursor(5, 41);
    ssd1306_write_string(buf, &Font_7x10, SSD1306_COLOR_WHITE);

    /* Line 5: Vao */
    snprintf(buf, sizeof(buf), "Vao: %d.%02d V", 
             (int)p_data->air_data.voltage_sensor, 
             (int)(p_data->air_data.voltage_sensor * 100.0f) % 100);
    ssd1306_set_cursor(5, 53);
    ssd1306_write_string(buf, &Font_7x10, SSD1306_COLOR_WHITE);
}

static void draw_bluetooth_screen(const display_service_data_t *p_data)
{
    ssd1306_set_cursor(5, 5);
    ssd1306_write_string("BLUETOOTH", &Font_7x10, SSD1306_COLOR_WHITE);

    ssd1306_set_cursor(5, 30);
    if (p_data->bluetooth_active)
    {
        ssd1306_write_string("TX ENABLED", &Font_7x10, SSD1306_COLOR_WHITE);
    }
    else
    {
        ssd1306_write_string("TX DISABLED", &Font_7x10, SSD1306_COLOR_WHITE);
    }
}

static void draw_error_screen(void)
{
    ssd1306_set_cursor(20, 25);
    ssd1306_write_string("SYS ERROR", &Font_11x18, SSD1306_COLOR_WHITE);
}
