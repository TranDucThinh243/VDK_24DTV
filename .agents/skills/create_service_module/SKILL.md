# Create STM32 Assets/Service Module Skill

## Purpose

Use this skill when creating or modifying an Assets/Service module.

A Service module processes device data and provides clean, stable data to the App layer.

## Layer

Target layer:

```text
Assets/Service
```

Allowed dependency:

```text
Assets/Service -> DeviceDrivers
```

Forbidden dependencies:

```text
Assets/Service must not call HAL directly.
Assets/Service should not call BSP directly unless explicitly approved.
Assets/Service must not include App headers.
Assets/Service must not own the entire main loop.
```

## Required Files

Create or modify exactly one service pair unless the user explicitly asks otherwise:

```text
Assets/Inc/<service_name>.h
Assets/Src/<service_name>.c
```

Examples:

```text
Assets/Inc/air_quality_service.h
Assets/Src/air_quality_service.c
Assets/Inc/health_service.h
Assets/Src/health_service.c
Assets/Inc/display_service.h
Assets/Src/display_service.c
Assets/Inc/bluetooth_service.h
Assets/Src/bluetooth_service.c
Assets/Inc/alert_service.h
Assets/Src/alert_service.c
```

## API Design Rules

Each service should expose:

1. A status enum using `_t` suffix.
2. A clean data struct if the service produces data.
3. An init function.
4. An update/process function.
5. Getter functions if needed.
6. Clear public comments.
7. Error handling.

Common API names:

```c
air_quality_service_init
air_quality_service_update
air_quality_service_get_data
health_service_init
health_service_update
health_service_get_data
display_service_update
bluetooth_service_send_health_data
alert_service_update
```

## Responsibility Rules

A service handles module-level processing.

Correct:

```text
air_quality_service converts MQ135 raw ADC to voltage or level.
health_service processes MAX30102 raw samples into clean health data.
display_service formats clean data for OLED.
bluetooth_service formats clean data for HC-05 transfer.
alert_service decides RGB/buzzer status from clean service data.
```

Wrong:

```text
Service directly calls HAL_ADC_Start.
Service directly writes I2C registers.
Service contains entire while(1) loop.
Service edits main.c.
```

## Data Struct Rule

Use explicit structs for clean data.

Example:

```c
typedef struct
{
    uint16_t raw_adc;
    float voltage;
    uint8_t level;
} air_quality_data_t;
```

## Error Handling Checklist

Every public function must check:

- NULL pointer
- driver return value
- invalid state
- invalid data pointer

Do not ignore driver return values.

## Output Requirements

When using this skill, respond with:

1. Layer confirmation
2. Service responsibility
3. Driver dependencies required
4. Files created or edited
5. Header code
6. Source code
7. Minimal service-level test
8. App integration notes

## Safety Rules

Do not modify:

- `main.c`
- `.ioc`
- BSP
- low-level drivers
- unrelated services

unless the user explicitly asks.
