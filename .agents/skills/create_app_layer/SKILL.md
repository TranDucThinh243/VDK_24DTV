# Create STM32 App Layer Skill

## Purpose

Use this skill when creating or modifying the App layer.

The App layer coordinates the system flow using service APIs.

## Layer

Target layer:

```text
App
```

Allowed dependency:

```text
App -> Assets/Service
```

Forbidden dependencies:

```text
App must not call HAL directly.
App must not call BSP directly.
App must not call DeviceDrivers directly unless explicitly approved.
App must not contain register-level logic.
```

## Required Files

Create or modify:

```text
App/Inc/app_main.h
App/Src/app_main.c
```

## Required API

The App layer must expose:

```c
void App_Init(void);
void App_Loop(void);
```

## main.c Integration Rule

`main.c` should remain clean and only call:

```c
App_Init();

while (1)
{
    App_Loop();
}
```

Do not put sensor, display, Bluetooth, buzzer, or algorithm logic directly inside `main.c`.

## Scheduling Rule

Prefer non-blocking timing.

Avoid blocking delays inside `App_Loop()`.

Recommended flow:

```text
- Read system tick
- Update sensor services by interval
- Update display service by interval
- Update Bluetooth service by interval
- Update alert service when needed
```

Example intervals:

```text
MAX30102 service: 100 ms
MQ135 service: 1000 ms
OLED display: 500 ms
Bluetooth send: 1000 ms
Alert service: 100 ms or event-based
```

## Responsibility Rules

Correct:

```text
App calls service init/update functions.
App manages application state.
App coordinates timing.
```

Wrong:

```text
App calls HAL_I2C_Mem_Read.
App reads MAX30102 registers directly.
App performs MQ135 ADC conversion directly.
App writes OLED buffer directly if display_service exists.
```

## Output Requirements

When using this skill, respond with:

1. Layer confirmation
2. App responsibilities
3. Services required
4. Files created or edited
5. Header code
6. Source code
7. Minimal main.c integration snippet
8. Build/debug notes

## Safety Rules

Do not modify:

- `.ioc`
- BSP
- DeviceDrivers
- Assets/Service

unless the user explicitly asks.

If editing `main.c`, only modify inside USER CODE blocks when working with CubeMX-generated files.
