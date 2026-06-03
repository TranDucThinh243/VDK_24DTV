---
description: One module one responsibility rules for STM32 project
globs:
  - "**/*.c"
  - "**/*.h"
alwaysApply: true
---

# Module Responsibility Rules

## One Module, One Main Responsibility

Each module must do one clear job.

Correct examples:

```text
bsp_i2c: low-level I2C wrapper only
bsp_uart: low-level UART wrapper only
bsp_adc: low-level ADC wrapper only
max30102: MAX30102 device control only
mq135: MQ135 device reading only
ssd1306: OLED device control only
health_service: clean health data processing only
air_quality_service: clean air quality data processing only
bluetooth_service: Bluetooth message formatting/sending only
app_main: application coordination only
```

## Forbidden Mixed Responsibilities

Wrong examples:

```text
max30102.c reads sensor and sends Bluetooth data
bsp_i2c.c calculates temperature or heart rate
mq135.c directly draws OLED UI
main.c contains all sensor and communication logic
bluetooth_service.c reads MAX30102 registers directly
```

## Driver vs Service Boundary

DeviceDrivers should expose raw or device-level data.

Services should process and convert data.

Example:

```text
mq135 driver:
- mq135_read_raw()
- mq135_read_voltage()

air_quality_service:
- air_quality_service_update()
- classify air quality level
- provide clean air_quality_data_t
```

## App vs Service Boundary

App should coordinate, not compute details.

Example:

```text
App: call air_quality_service_update()
Service: convert raw ADC to clean result
Driver: read MQ135 voltage/raw value
BSP: read ADC through HAL
```

## Self-Check

Before finalizing code, check:

1. Does this module have one clear job?
2. Is any unrelated feature mixed into this file?
3. Is a lower-layer file doing high-level work?
4. Is a higher-layer file doing low-level work?
5. Should this logic be moved to Service or App?
