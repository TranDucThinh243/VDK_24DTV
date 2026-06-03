---
description: Folder and file placement rules for STM32 project modules
globs:
  - "**/*.c"
  - "**/*.h"
alwaysApply: true
---

# Folder Structure Rules

## Required Project Module Folders

The project must use this structure for user code:

```text
App/
  Inc/
  Src/

Assets/
  Inc/
  Src/

BSP/
  Inc/
  Src/

DeviceDrivers/
  Inc/
  Src/
```

## File Placement

Each module must have one `.h` file and one `.c` file.

Header files go in `Inc/`.
Source files go in `Src/`.

Examples:

```text
BSP/Inc/bsp_i2c.h
BSP/Src/bsp_i2c.c

DeviceDrivers/Inc/max30102.h
DeviceDrivers/Src/max30102.c

Assets/Inc/health_service.h
Assets/Src/health_service.c

App/Inc/app_main.h
App/Src/app_main.c
```

## CubeMX Generated Code

Do not move or rewrite generated CubeMX files unless explicitly requested.

Generated folders such as these should remain under CubeMX control:

```text
Core/
Drivers/
Middlewares/
```

When editing generated files, edit only inside `USER CODE` sections unless the user explicitly approves another change.

## Adding New Files

When creating a new module:

1. Create the header in the correct `Inc/` folder.
2. Create the source in the correct `Src/` folder.
3. Use file names that match the module responsibility.
4. Tell the user to check include paths in STM32CubeIDE.
5. Tell the user to check that the `.c` file is not excluded from build.

## Forbidden File Placement

Do not place user modules randomly inside `Core/Src` or `Core/Inc` unless the user explicitly requests it.

Do not put all modules in `main.c`.

Do not create mixed-responsibility files such as:

```text
sensor_and_bluetooth.c
all_drivers.c
project_logic.c
main_app_everything.c
```
