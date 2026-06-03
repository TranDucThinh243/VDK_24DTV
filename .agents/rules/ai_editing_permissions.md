---
description: Agent editing permissions and safety rules for STM32 project
globs:
  - "**/*"
alwaysApply: true
---

# AI Editing Permission Rules

## General Rule

Before editing, state which files will be created or modified.

Do not modify unrelated files.

Do not rewrite the whole project unless the user explicitly requests it.

## Allowed Without Extra Approval

The agent may create or edit files in these project-owned folders when the user asks for a related module:

```text
App/Inc/
App/Src/
Assets/Inc/
Assets/Src/
BSP/Inc/
BSP/Src/
DeviceDrivers/Inc/
DeviceDrivers/Src/
.agents/rules/
.agents/skills/
```

## Requires Explicit Approval

The agent must ask before modifying:

```text
Core/Src/main.c
Core/Inc/main.h
*.ioc
.project
.cproject
startup files
linker scripts
Drivers/
Middlewares/
Git configuration
CI/build scripts
```

## Never Do Silently

Do not silently:

- Delete files.
- Rename folders.
- Change pin mapping.
- Change CubeMX peripheral configuration.
- Change clock configuration.
- Change startup/linker files.
- Replace a module owned by another teammate.
- Push to Git.
- Install packages or tools.

## main.c Special Rule

`main.c` can only be changed when:

- The user explicitly asks, or
- The only change is adding `App_Init()` and `App_Loop()` in the correct `USER CODE` sections, and the user approves.

## Pin Mapping Rule

The current hardware context must not be changed unless explicitly requested.

Current pin mapping:

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

## Response Before Editing

Before editing, provide:

1. Target module
2. Layer
3. Files to create/edit
4. Files not to touch
5. Whether approval is needed

## Self-Check

Before finalizing, check:

1. Did the edit stay within allowed files?
2. Did the edit avoid generated files unless approved?
3. Did the edit preserve pin mapping?
4. Did the edit avoid deleting or renaming unrelated files?
5. Did the edit avoid rewriting too much code?
