---
description: main.c and App layer rules for STM32 project integration
globs:
  - "**/main.c"
  - "App/**/*.c"
  - "App/**/*.h"
alwaysApply: true
---

# main.c and App Layer Rules

## main.c Must Stay Clean

`main.c` should only contain:

- HAL initialization
- System clock configuration
- CubeMX peripheral initialization
- `App_Init()`
- `App_Loop()` inside `while (1)`

Expected style:

```c
int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    MX_ADC1_Init();

    App_Init();

    while (1)
    {
        App_Loop();
    }
}
```

## Forbidden in main.c

Do not place these in `main.c`:

- Sensor register access
- OLED drawing logic
- Bluetooth protocol logic
- ADC conversion processing
- MAX30102 algorithm logic
- MQ135 conversion logic
- Large state machines
- Long blocking loops
- Driver/service implementation

## App Layer Responsibilities

The App layer coordinates system behavior.

Allowed in App:

- Calling service init functions
- Calling service update functions
- Simple timing coordination
- State machine coordination
- Calling `App_Init()` and `App_Loop()`

Forbidden in App:

- Direct HAL calls
- Direct BSP calls
- Device register access
- Raw sensor conversion that belongs in service
- Heavy peripheral logic

## Non-Blocking Loop Preference

Prefer non-blocking update intervals over `HAL_Delay()` in `App_Loop()`.

Recommended pattern:

```c
void App_Loop(void)
{
    uint32_t now_ms = app_time_get_ms();

    if ((now_ms - s_last_sensor_ms) >= APP_SENSOR_INTERVAL_MS)
    {
        s_last_sensor_ms = now_ms;
        sensor_service_update();
    }
}
```

If a tick source is required, access it through a service or BSP wrapper, not direct HAL from App.

## Integration Rule

When integrating a new module:

1. Test BSP first.
2. Test DeviceDriver next.
3. Test Service next.
4. Integrate with App last.
5. Modify `main.c` only if needed to add `App_Init()` / `App_Loop()` calls.

## Self-Check

Before finalizing App changes, check:

1. Did `main.c` stay clean?
2. Does App call only services?
3. Is timing non-blocking where possible?
4. Are raw device details hidden below App?
5. Is the App logic easy to understand?
