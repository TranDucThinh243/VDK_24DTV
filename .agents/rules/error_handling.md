---
description: Error handling and defensive programming rules for STM32 modules
globs:
  - "**/*.c"
  - "**/*.h"
alwaysApply: true
---

# Error Handling Rules

## Mandatory Error Checks

Every function that can fail must check relevant errors.

Always check:

- NULL pointers
- `size == 0U`
- invalid enum values
- invalid handles
- HAL return values
- BSP return values
- Driver return values

## Do Not Ignore Return Values

If a called function returns a status, check it.

Correct:

```c
status = bsp_i2c_mem_read(dev_addr, reg_addr, p_data, size, timeout_ms);
if (status != BSP_I2C_OK)
{
    return MAX30102_ERROR;
}
```

Wrong:

```c
bsp_i2c_mem_read(dev_addr, reg_addr, p_data, size, timeout_ms);
return MAX30102_OK;
```

## NULL Pointer Check

Correct:

```c
if (p_data == NULL)
{
    return MODULE_ERROR;
}
```

For buffers:

```c
if (p_data == NULL || size == 0U)
{
    return MODULE_ERROR;
}
```

## Status Type Per Module

Each module that can fail should define its own status enum.

Example:

```c
typedef enum
{
    MAX30102_OK = 0,
    MAX30102_ERROR,
    MAX30102_INVALID_PARAM,
    MAX30102_COMM_ERROR
} max30102_status_t;
```

## Status Conversion

Convert lower-layer errors into current-layer errors.

Example:

```c
if (bsp_status != BSP_I2C_OK)
{
    return MAX30102_COMM_ERROR;
}
```

## Avoid Silent Failure

Do not hide errors unless the design explicitly requires it.

If a module cannot finish its operation, return an error status.

## Self-Check

Before finalizing code, check:

1. Are all pointers validated?
2. Are buffer sizes validated?
3. Are lower-layer status values checked?
4. Does the function return meaningful status?
5. Are errors converted at layer boundaries?
6. Is any failure silently ignored?
