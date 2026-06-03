---
description: Build and debug workflow rules for STM32 project modules
globs:
  - "**/*.c"
  - "**/*.h"
  - "**/Makefile"
  - "**/.cproject"
alwaysApply: true
---

# Build and Debug Rules

## Build Rules

Generated or edited code must be likely to build.

When adding new files, remind the user to check:

- `.c` file is not excluded from build.
- Include path is configured in STM32CubeIDE.
- Header has include guard.
- Function prototype matches implementation.
- Required HAL handle exists.
- Required peripheral is enabled in CubeMX.
- No circular include exists.

## Do Not Break CubeMX Project

Do not modify `.ioc`, `.project`, `.cproject`, linker script, startup file, or CubeMX-generated files unless explicitly requested.

If a change requires CubeMX configuration, explain what the user needs to enable instead of silently editing generated configuration.

## Minimal Test Rule

Every new module must include a minimal test plan.

Example test order for an I2C sensor:

1. Build empty project.
2. Add BSP I2C and build.
3. Test `bsp_i2c_is_device_ready()`.
4. Add device driver and build.
5. Test read ID/register.
6. Add service and build.
7. Integrate with App.

## Debug by Layer

Debug in this order:

```text
1. Hardware wiring
2. CubeMX peripheral configuration
3. HAL handle exists and is initialized
4. BSP function works
5. DeviceDriver function works
6. Assets/Service processing works
7. App flow works
8. main.c integration is correct
```

## Do Not Debug Randomly

Do not jump to high-level code if the lower layer has not been tested.

Examples:

- If I2C sensor cannot read ID, test wiring, address, CubeMX I2C config, and BSP before service.
- If UART Bluetooth does not send, test UART BSP before Bluetooth service.
- If ADC value is wrong, test ADC BSP before MQ135 service.

## Build Error Response Format

When the user provides a build error, respond with:

1. Error category
2. Most likely file/layer
3. Cause
4. Minimal fix
5. How to verify

## Self-Check

Before finalizing changes, check:

1. Is the code likely to compile?
2. Are include paths clear?
3. Are needed handles named explicitly?
4. Are generated files left safe?
5. Is there a minimal test plan?
