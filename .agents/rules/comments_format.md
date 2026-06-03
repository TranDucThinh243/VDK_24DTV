---
description: Comment and formatting rules for STM32 C code
globs:
  - "**/*.c"
  - "**/*.h"
alwaysApply: true
---

# Comment and Format Rules

## File Header Comment

Each `.h` and `.c` file must have a short file header comment.

Example:

```c
/**
 * @file bsp_i2c.c
 * @brief BSP wrapper for STM32 HAL I2C functions.
 * @version 1.0.0
 */
```

## Public Function Comment

Each public function must have a comment containing:

- brief
- input parameters
- output parameters
- return value

Example:

```c
/**
 * @brief Read memory from an I2C device.
 * @param[in] dev_addr I2C device address.
 * @param[in] mem_addr Register or memory address.
 * @param[out] p_data Pointer to output buffer.
 * @param[in] size Number of bytes to read.
 * @return bsp_i2c_status_t
 */
```

## Comment Quality

Comment only what helps humans understand the code.

Do not write meaningless comments.

Wrong:

```c
i++; /* Increase i */
```

Better:

```c
/* Skip the first FIFO byte because it contains the tag field. */
```

## Brace Rules

Always use braces for control blocks.

Correct:

```c
if (status != BSP_I2C_OK)
{
    return MAX30102_ERROR;
}
```

Wrong:

```c
if (status != BSP_I2C_OK)
    return MAX30102_ERROR;
```

## Spacing Rules

Use clear spacing.

Correct:

```c
if (hi2c == NULL || p_data == NULL)
```

Wrong:

```c
if(hi2c==NULL||p_data==NULL)
```

## Line Length

Avoid overly long lines.

If a function call or condition becomes too long, split it cleanly across multiple lines.

## Style Consistency

Use this brace style consistently:

```c
if (condition)
{
    do_something();
}
```

## Self-Check

Before finalizing code, check:

1. Does each file have a file header?
2. Does each public function have useful documentation?
3. Are braces always used?
4. Is spacing readable?
5. Are comments useful rather than noisy?
