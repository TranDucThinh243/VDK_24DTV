/**
 * @file app_main.c
 * @brief Application Layer Main Logic Source
 * @version 1.0.0
 */

#include "app_main.h"

/* Services */
#include "air_quality_service.h"
#include "display_service.h"
#include "bluetooth_service.h"

/* Time dependency for non-blocking scheduler */
#include "bsp_time.h"

/* Task execution intervals in milliseconds */
#define TASK_INTERVAL_AIR_QUALITY_MS    1000U
#define TASK_INTERVAL_DISPLAY_MS        500U
#define TASK_INTERVAL_BLUETOOTH_MS      1000U

/* Static variables to track the last execution time of each task */
static uint32_t s_last_air_quality_tick = 0;
static uint32_t s_last_display_tick = 0;
static uint32_t s_last_bluetooth_tick = 0;

/* Global application state to pass between services */
static display_service_data_t s_display_data = {0};

/* Private function prototypes */
static void app_task_air_quality(void);
static void app_task_display(void);
static void app_task_bluetooth(void);

/* Public functions */

void App_Init(void)
{
    /* Initialize all services */
    air_quality_service_init();
    display_service_init();
    bluetooth_service_init();
    
    /* Set default display screen */
    display_service_set_screen(DISPLAY_SCREEN_AIR);
    
    /* Initialize task timers to current tick so they run immediately on start */
    uint32_t current_tick = bsp_time_get_ms();
    
    /* Force immediate execution on first loop by offsetting the last tick */
    s_last_air_quality_tick = current_tick - TASK_INTERVAL_AIR_QUALITY_MS;
    s_last_display_tick = current_tick - TASK_INTERVAL_DISPLAY_MS;
    s_last_bluetooth_tick = current_tick - TASK_INTERVAL_BLUETOOTH_MS;
}

void App_Loop(void)
{
    uint32_t current_tick = bsp_time_get_ms();

    /* Non-blocking Air Quality Task */
    if (bsp_time_is_elapsed(s_last_air_quality_tick, TASK_INTERVAL_AIR_QUALITY_MS))
    {
        s_last_air_quality_tick = current_tick;
        app_task_air_quality();
    }

    /* Non-blocking Display Task */
    if (bsp_time_is_elapsed(s_last_display_tick, TASK_INTERVAL_DISPLAY_MS))
    {
        s_last_display_tick = current_tick;
        app_task_display();
    }

    /* Non-blocking Bluetooth Task */
    if (bsp_time_is_elapsed(s_last_bluetooth_tick, TASK_INTERVAL_BLUETOOTH_MS))
    {
        s_last_bluetooth_tick = current_tick;
        app_task_bluetooth();
    }
}

/* Private functions */

static void app_task_air_quality(void)
{
    /* 1. Request the service to update its internal data from the sensor */
    air_quality_service_update();
    
    /* 2. Retrieve the fresh clean data into our application state */
    air_quality_service_get_data(&s_display_data.air_data);
}

static void app_task_display(void)
{
    /* Update display state tracking based on system health */
    s_display_data.bluetooth_active = (bluetooth_service_get_status() == BLUETOOTH_SERVICE_OK);
    s_display_data.system_error = 0; /* Placeholder: could track if any service failed */

    /* Pass the aggregated data struct down to the display service for rendering */
    display_service_update(&s_display_data);
}

static void app_task_bluetooth(void)
{
    /* Transmit the current air quality data via Bluetooth */
    bluetooth_service_send_air_quality(&s_display_data.air_data);
}
