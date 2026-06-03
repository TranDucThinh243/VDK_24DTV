---
description: Mandatory STM32 layered architecture rules for this project
globs:
  - "**/*.c"
  - "**/*.h"
alwaysApply: true
---

# STM32 Architecture Rules

## Mandatory Architecture

All production code must follow this layered architecture:

```text
App -> Assets/Service -> DeviceDrivers -> BSP -> HAL
```

## Layer Responsibilities

### App

The App layer coordinates application flow only.

Allowed responsibilities:

- Call service initialization functions.
- Call service update functions.
- Coordinate timing and application state.
- Provide `App_Init()` and `App_Loop()`.

Forbidden responsibilities:

- Direct HAL calls.
- Direct BSP calls.
- Direct sensor register access.
- Direct OLED, Bluetooth, ADC, I2C, UART, or GPIO logic.
- Device-specific algorithms that belong in services.

### Assets/Service

The Assets/Service layer processes clean module-level data.

Allowed responsibilities:

- Convert raw sensor data into useful values.
- Filter, validate, and format data.
- Coordinate closely related DeviceDrivers.
- Provide clean data to App.

Forbidden responsibilities:

- Direct HAL calls.
- Application main loop ownership.
- Unrelated device control.
- Editing `main.c`.

### DeviceDrivers

DeviceDrivers control one specific external device/module.

Allowed responsibilities:

- Read/write device registers.
- Initialize device-specific configuration.
- Provide device-level APIs.
- Call BSP functions.

Forbidden responsibilities:

- Direct HAL calls.
- App logic.
- Service-level data processing.
- Bluetooth sending from a sensor driver.
- OLED drawing from a sensor driver.

### BSP

BSP wraps STM32 HAL and provides low-level hardware access.

Allowed responsibilities:

- Call HAL functions.
- Wrap I2C, UART, ADC, GPIO, Timer, tick, delay.
- Convert HAL status to project status.

Forbidden responsibilities:

- Sensor-specific logic.
- Display-specific logic.
- App/service logic.
- Including DeviceDrivers or App headers.

## Allowed Dependencies

```text
App may include/call Assets/Service.
Assets/Service may include/call DeviceDrivers.
DeviceDrivers may include/call BSP.
BSP may include/call HAL.
```

## Forbidden Dependencies

```text
App must not call HAL directly.
App must not call BSP directly unless explicitly approved.
App must not call DeviceDrivers directly unless explicitly approved.
Assets/Service must not call HAL directly.
DeviceDrivers must not call HAL directly.
DeviceDrivers must not include App headers.
BSP must not include DeviceDrivers headers.
BSP must not include App headers.
```

## Required Self-Check Before Editing

Before editing or creating a file, answer internally:

1. Which layer does this module belong to?
2. Which lower layer is it allowed to call?
3. Does the requested change violate the architecture?
4. Does the change put logic into the wrong layer?
5. Does the change keep `main.c` clean?

If a user request conflicts with these rules, explain the conflict and propose a compliant structure.
