# Create STM32 Device Driver Skill

## Purpose

Use this skill when creating or modifying a DeviceDrivers module for a specific hardware component such as a sensor, display, Bluetooth module, or external chip.

A Device Driver controls one specific device and hides register-level or command-level details from upper layers.

## Layer

Target layer:

```text
DeviceDrivers
```

Allowed dependency:

```text
DeviceDrivers -> BSP
```

Forbidden dependencies:

```text
DeviceDrivers must not call HAL directly.
DeviceDrivers must not include App headers.
DeviceDrivers must not include Assets/Service headers unless explicitly approved.
DeviceDrivers must not contain whole application flow.
```

## Required Files

Create or modify exactly one driver pair unless the user explicitly asks otherwise:

```text
DeviceDrivers/Inc/<device_name>.h
DeviceDrivers/Src/<device_name>.c
```

Examples:

```text
DeviceDrivers/Inc/max30102.h
DeviceDrivers/Src/max30102.c
DeviceDrivers/Inc/mq135.h
DeviceDrivers/Src/mq135.c
DeviceDrivers/Inc/ssd1306.h
DeviceDrivers/Src/ssd1306.c
DeviceDrivers/Inc/hc05.h
DeviceDrivers/Src/hc05.c
```

## API Design Rules

Each driver must expose:

1. A status enum using `_t` suffix.
2. A config/device struct if state or addresses are needed.
3. An init function.
4. Read/write/control functions.
5. Clear public comments.
6. Basic error handling.

Example API names:

```c
max30102_init
max30102_read_part_id
max30102_reset
max30102_read_fifo
mq135_init
mq135_read_raw
ssd1306_init
ssd1306_clear
hc05_send
hc05_receive
```

## Responsibility Rules

A driver must do only one device-level job.

Correct:

```text
max30102.c reads/writes MAX30102 registers and FIFO.
mq135.c reads MQ135 raw ADC data through BSP ADC.
ssd1306.c controls OLED drawing commands and buffer update.
hc05.c wraps HC-05 UART communication.
```

Wrong:

```text
max30102.c calculates final health diagnosis.
mq135.c sends Bluetooth messages.
ssd1306.c reads sensors directly.
hc05.c controls OLED display.
```

## Register and Macro Rules

Use uppercase macros for register addresses and constants.

Example:

```c
#define MAX30102_REG_PART_ID      0xFFU
#define MAX30102_EXPECTED_PART_ID 0x15U
```

Keep register macros private in `.c` unless other modules truly need them.

## Error Handling Checklist

Every public function must check:

- NULL pointer
- invalid size
- invalid config/device pointer
- BSP return value

Do not ignore BSP return values.

## Output Requirements

When using this skill, respond with:

1. Layer confirmation
2. Device responsibility
3. BSP dependencies required
4. Files created or edited
5. Header code
6. Source code
7. Minimal device-level test
8. Integration notes for Service layer

## Safety Rules

Do not modify:

- `main.c`
- `.ioc`
- App layer
- unrelated drivers
- unrelated BSP modules

unless the user explicitly asks.
