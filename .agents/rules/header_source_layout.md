---
description: Required header and source layout rules for STM32 C modules
globs:
  - "**/*.c"
  - "**/*.h"
alwaysApply: true
---

# Header and Source Layout Rules

## Header File Requirements

Each `.h` file must contain:

1. File header comment
2. Include guard
3. Minimal includes
4. Public macros if needed
5. Public typedef/enum/struct if needed
6. Public function prototypes only

## Header Guard Pattern

Use this style:

```c
#ifndef BSP_I2C_H
#define BSP_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Public content */

#ifdef __cplusplus
}
#endif

#endif /* BSP_I2C_H */
```

The guard name must match the file name in uppercase.

Examples:

```text
bsp_i2c.h -> BSP_I2C_H
max30102.h -> MAX30102_H
health_service.h -> HEALTH_SERVICE_H
app_main.h -> APP_MAIN_H
```

## Header Restrictions

Do not put normal function bodies in `.h` files.

Do not expose private helper functions in `.h` files.

Do not expose private registers unless another module genuinely needs them.

## Source File Layout

Each `.c` file should follow this layout:

```c
/**
 * @file module_name.c
 * @brief Short description.
 * @version 1.0.0
 */

/* Includes */
#include "module_name.h"

/* Private defines */

/* Private typedef */

/* Private variables */

/* Private function prototypes */

/* Public function definitions */

/* Private function definitions */
```

## Public and Private Functions

Public functions:

- Declared in `.h`
- Defined in `.c`
- Must not be `static`
- Must have documentation comment

Private functions:

- Declared only in `.c`
- Must be `static`
- Should be placed in Private function prototypes section

## Self-Check

Before finalizing code, check:

1. Does every header have an include guard?
2. Are only public APIs in the header?
3. Are private helpers marked `static`?
4. Does the source file follow the required layout?
5. Are function prototypes and definitions consistent?
