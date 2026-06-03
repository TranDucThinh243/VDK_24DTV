---
description: Hardware context and pin mapping for the STM32F103C8T6 project
globs:
  - "**/*.c"
  - "**/*.h"
  - "**/*.ioc"
alwaysApply: true
---

# Hardware Context Rules

## Target MCU

Main target board:

```text
STM32F103C8T6 Bluepill
```

Framework:

```text
STM32 HAL
```

## Known Clock Context

Expected project clock context:

```text
HCLK: 72 MHz
APB1: 36 MHz
```

Do not change clock configuration unless explicitly requested.

## Current Hardware Modules

The project may include these hardware modules:

```text
SSD1306 OLED via I2C
MAX30102 via I2C
MQ135 via ADC
HC-05 Bluetooth via UART
RGB LED via GPIO/PWM
Buzzer via GPIO/PWM
Button 1
Button 2
Touch input
```

## Pin Mapping

Do not change this pin mapping unless the user explicitly requests it:

```text
PA0: MQ135 ADC
PA1: RGB channel 1
PA2: RGB channel 2
PA3: RGB channel 3
PA8: Buzzer
PA9: UART TX to HC-05 RX
PA10: UART RX from HC-05 TX
PB6: I2C SCL
PB7: I2C SDA
PB12: Button 1
PB13: Button 2
PB14: Touch input
```

## Common HAL Handles

Use these common handle names only when they match the existing project:

```text
hi2c1: I2C1, usually PB6/PB7
huart1: USART1, usually PA9/PA10
hadc1: ADC1, usually PA0
```

If a handle is not confirmed in code, inspect the project or state the assumption clearly.

## Hardware Configuration Rule

If a module requires CubeMX setup, explain it clearly.

Do not silently edit `.ioc`.

Example:

```text
To use MQ135 on PA0, ADC1 channel for PA0 must be enabled in CubeMX.
```

## Self-Check

Before finalizing hardware-related code, check:

1. Did the code preserve pin mapping?
2. Did the code use the correct peripheral concept?
3. Are required handles available?
4. Does any change require CubeMX configuration?
5. Was any assumption stated clearly?
