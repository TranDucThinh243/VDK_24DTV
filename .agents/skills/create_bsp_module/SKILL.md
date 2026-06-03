# Create STM32 BSP Module Skill

## Purpose

Use this skill when creating or modifying a BSP module for an STM32 HAL project.

A BSP module wraps STM32 HAL and exposes a clean, project-specific API to upper layers.

## Layer

Target layer:

```text
BSP
```

Allowed dependency:

```text
BSP -> HAL
```

Forbidden dependencies:

```text
BSP must not include DeviceDrivers headers.
BSP must not include Assets/Service headers.
BSP must not include App headers.
```

## Required Files

Create or modify exactly one module pair unless the user explicitly asks otherwise:

```text
BSP/Inc/bsp_<peripheral>.h
BSP/Src/bsp_<peripheral>.c
```

Examples:

```text
BSP/Inc/bsp_i2c.h
BSP/Src/bsp_i2c.c
BSP/Inc/bsp_uart.h
BSP/Src/bsp_uart.c
BSP/Inc/bsp_adc.h
BSP/Src/bsp_adc.c
BSP/Inc/bsp_gpio.h
BSP/Src/bsp_gpio.c
BSP/Inc/bsp_time.h
BSP/Src/bsp_time.c
```

## API Design Rules

Each BSP module must expose:

1. A status enum using `_t` suffix.
2. Public functions using `module_action` naming.
3. Minimal HAL wrapper APIs.
4. Error handling for invalid input.
5. Mapping from HAL status to BSP status when relevant.

Example status enum:

```c
typedef enum
{
    BSP_I2C_OK = 0,
    BSP_I2C_ERROR,
    BSP_I2C_TIMEOUT,
    BSP_I2C_INVALID_PARAM
} bsp_i2c_status_t;
```

Example APIs:

```c
bsp_i2c_status_t bsp_i2c_mem_read(I2C_HandleTypeDef *hi2c,
                                   uint16_t dev_addr,
                                   uint16_t mem_addr,
                                   uint16_t mem_addr_size,
                                   uint8_t *p_data,
                                   uint16_t size,
                                   uint32_t timeout_ms);
```

## Error Handling Checklist

Every public function must check:

- NULL pointer
- `size == 0U` where relevant
- invalid HAL handle
- HAL return value
- timeout result if relevant

Do not ignore HAL return values.

## Source Layout

Use this layout in `.c` files:

```c
/* Includes */

/* Private defines */

/* Private typedef */

/* Private variables */

/* Private function prototypes */

/* Public function definitions */

/* Private function definitions */
```

Private helpers must be `static`.

## Header Rules

Each `.h` file must include:

- Include guard
- Minimal includes
- Public typedefs
- Public prototypes only

Do not put normal function bodies in headers.

## Output Requirements

When using this skill, respond with:

1. Layer confirmation
2. Module responsibility
3. Files created or edited
4. Header code
5. Source code
6. Required CubeIDE include paths
7. Minimal test steps
8. Build/debug notes

## Safety Rules

Do not modify:

- `main.c`
- `.ioc`
- DeviceDrivers
- Assets/Service
- App

unless the user explicitly asks.
