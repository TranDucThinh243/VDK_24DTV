---
description: Include rules and dependency hygiene for STM32 C modules
globs:
  - "**/*.c"
  - "**/*.h"
alwaysApply: true
---

# Include Rules

## General Include Rule

Only include what is truly needed.

Do not include files casually.

## Include Path Rule

Do not use relative include paths such as:

```c
#include "../BSP/Inc/bsp_i2c.h"
#include "../../DeviceDrivers/Inc/max30102.h"
```

Use clean includes:

```c
#include "bsp_i2c.h"
#include "max30102.h"
```

The include paths must be configured in STM32CubeIDE or the build system.

## Correct Include Direction

Allowed examples:

```c
/* DeviceDrivers/Src/max30102.c */
#include "max30102.h"
#include "bsp_i2c.h"

/* Assets/Src/health_service.c */
#include "health_service.h"
#include "max30102.h"

/* App/Src/app_main.c */
#include "app_main.h"
#include "health_service.h"
```

## Forbidden Includes

Forbidden examples:

```c
/* BSP must not know about drivers. */
#include "max30102.h"

/* DeviceDrivers must not know about App. */
#include "app_main.h"

/* App should not include HAL for peripheral access. */
#include "stm32f1xx_hal_i2c.h"
```

## Header Include Minimalism

A header file should include only what it needs for type declarations.

Prefer forward declarations when possible.

Do not include large HAL headers in upper-layer headers unless truly required.

## Source Include Layout

Recommended `.c` include order:

```c
#include "module_name.h"

#include "lower_layer_dependency.h"

#include <stdint.h>
#include <stddef.h>
```

## Self-Check

Before finalizing code, check:

1. Are all includes necessary?
2. Is any include going upward in the architecture?
3. Is any file using `../` include paths?
4. Is HAL included outside BSP without justification?
5. Can include paths be configured cleanly in the IDE?
