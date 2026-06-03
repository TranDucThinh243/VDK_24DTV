---
description: Naming conventions for STM32 C files, functions, types, macros, and variables
globs:
  - "**/*.c"
  - "**/*.h"
alwaysApply: true
---

# Naming Rules

## File Names

File names must be:

- lowercase
- underscore-separated
- short
- meaningful

Correct examples:

```text
bsp_i2c.h
bsp_i2c.c
bsp_uart.h
bsp_uart.c
max30102.h
max30102.c
mq135.h
mq135.c
health_service.h
health_service.c
app_main.h
app_main.c
```

Wrong examples:

```text
BSP_I2C.c
SensorCode.c
mycode.c
function.c
all_in_one.c
```

## Function Names

Use this pattern:

```text
module_action
```

Correct examples:

```c
bsp_i2c_mem_read
bsp_i2c_mem_write
bsp_uart_transmit
max30102_read_part_id
mq135_read_raw
health_service_update
app_main_loop
```

Wrong examples:

```c
ReadSensor
getData
send
function1
XuLyCamBien
```

## Type Names

Use `_t` suffix for typedefs, structs, and enums.

Correct examples:

```c
bsp_i2c_status_t
max30102_status_t
mq135_data_t
health_data_t
app_state_t
```

## Macro Names

Macros must be uppercase with underscores.

Correct examples:

```c
BSP_I2C_TIMEOUT_MS
MAX30102_REG_PART_ID
MQ135_ADC_MAX_VALUE
APP_SENSOR_UPDATE_INTERVAL_MS
```

## Variable Names

Local variables:

- lowercase
- underscore-separated

Correct examples:

```c
chip_id
raw_adc
timeout_ms
line_buffer
```

Static file-scope variables:

```c
s_oled_buffer
s_latest_data
```

Global variables, only if truly necessary:

```c
g_health_data
g_system_state
```

## Naming Self-Check

Before finalizing code, check:

1. Does the file name reflect one clear responsibility?
2. Are functions named `module_action`?
3. Do typedef names end in `_t`?
4. Are macros uppercase?
5. Are local variables readable and consistent?
6. Are global/static variables prefixed clearly?
