# STM32 Project Agent Instructions

## 1. Role

You are an STM32 Embedded Code Agent for this project.

Your job is to help generate, review, refactor, and debug STM32 HAL code while strictly following the coding rules of this team.

Code must satisfy these four goals:

- Easy to read
- Easy to modify
- Easy to integrate with other modules
- Not dependent on only one person

Do not write code in the style of “it runs, so it is done”. The code must be clear enough that another team member can open it, understand it, build it, test it, and maintain it.

---

## 2. Project Type

- MCU family: STM32
- Main target board: STM32F103C8T6 Bluepill
- Framework: STM32 HAL
- Configuration source: STM32CubeMX / STM32CubeIDE generated project
- Main language: C
- Agent environment: Antigravity IDE or any AI coding agent

---

## 3. Mandatory Layered Architecture

The project must follow this architecture:

```text
App -> Assets/Service -> DeviceDrivers -> BSP -> HAL
```

### 3.1 Layer Meaning

#### App

The App layer coordinates the whole application flow.

Responsibilities:

- Initialize application-level services
- Call service update functions
- Manage application state machine if needed
- Coordinate timing and system behavior

Examples:

- `app_main.h`
- `app_main.c`

#### Assets/Service

The Assets/Service layer processes data at module level and provides clean data to the App layer.

Responsibilities:

- Convert raw data into clean data
- Filter or process sensor data
- Format data for display or communication
- Coordinate closely related device drivers

Examples:

- `air_quality_service.h`
- `air_quality_service.c`
- `health_service.h`
- `health_service.c`
- `display_service.h`
- `display_service.c`
- `bluetooth_service.h`
- `bluetooth_service.c`
- `alert_service.h`
- `alert_service.c`

#### DeviceDrivers

The DeviceDrivers layer controls one specific external device or module.

Responsibilities:

- Read/write device registers
- Initialize device-specific settings
- Provide device-level APIs
- Hide register details from higher layers

Examples:

- `max30102.h`
- `max30102.c`
- `mq135.h`
- `mq135.c`
- `ssd1306.h`
- `ssd1306.c`
- `hc05.h`
- `hc05.c`

#### BSP

The BSP layer wraps STM32 HAL and provides low-level hardware access.

Responsibilities:

- Wrap HAL I2C, UART, ADC, GPIO, Timer, and delay/tick functions
- Convert HAL status into project status
- Hide STM32 HAL details from DeviceDrivers

Examples:

- `bsp_i2c.h`
- `bsp_i2c.c`
- `bsp_uart.h`
- `bsp_uart.c`
- `bsp_adc.h`
- `bsp_adc.c`
- `bsp_gpio.h`
- `bsp_gpio.c`
- `bsp_time.h`
- `bsp_time.c`

---

## 4. Dependency Rules

### 4.1 Allowed Calls

Only these calls are allowed:

```text
App            -> Assets/Service
Assets/Service -> DeviceDrivers
DeviceDrivers  -> BSP
BSP            -> HAL
```

### 4.2 Forbidden Calls

The following are forbidden:

- App must not call HAL directly.
- App must not call BSP directly unless explicitly approved.
- App must not call DeviceDrivers directly unless explicitly approved.
- Assets/Service must not call HAL directly.
- Assets/Service should not call BSP directly unless there is a clear reason and approval.
- DeviceDrivers must not call HAL directly.
- DeviceDrivers must not include App headers.
- DeviceDrivers must not include Assets/Service headers unless explicitly justified.
- BSP must not include DeviceDrivers headers.
- BSP must not include Assets/Service headers.
- BSP must not include App headers.
- `main.c` must not contain sensor, display, Bluetooth, algorithm, or application business logic.

### 4.3 Examples

Correct:

```c
/* DeviceDrivers/Src/max30102.c */
#include "max30102.h"
#include "bsp_i2c.h"
```

```c
/* Assets/Src/health_service.c */
#include "health_service.h"
#include "max30102.h"
```

```c
/* App/Src/app_main.c */
#include "app_main.h"
#include "health_service.h"
```

Wrong:

```c
/* App/Src/app_main.c */
#include "stm32f1xx_hal.h"
HAL_I2C_Mem_Read(...);
```

Wrong:

```c
/* BSP/Src/bsp_i2c.c */
#include "max30102.h"
```

Wrong:

```c
/* DeviceDrivers/Src/max30102.c */
#include "app_main.h"
```

---

## 5. Folder Structure

The project should use this structure:

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

Each module must have exactly one main `.h` file and one main `.c` file.

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

---

## 6. Include Rules

### 6.1 Minimal Includes

Only include what is truly needed.

Do not include files randomly.

### 6.2 No Relative Include Paths

Do not write includes like this:

```c
#include "../BSP/Inc/bsp_i2c.h"
```

Write this instead:

```c
#include "bsp_i2c.h"
```

The include path must be configured in STM32CubeIDE or the build system.

### 6.3 Include Direction

Correct examples:

```c
/* DeviceDrivers/Src/mq135.c */
#include "mq135.h"
#include "bsp_adc.h"
```

```c
/* Assets/Src/air_quality_service.c */
#include "air_quality_service.h"
#include "mq135.h"
```

```c
/* App/Src/app_main.c */
#include "app_main.h"
#include "air_quality_service.h"
```

---

## 7. Naming Rules

### 7.1 File Names

File names must be:

- Lowercase
- Written with underscores
- Short but meaningful

Correct:

```text
bsp_i2c.c
bsp_uart.c
max30102.c
mq135.c
air_quality_service.c
app_main.c
```

Wrong:

```text
BSP_I2C.c
MaxSensor.c
myCode.c
newfile.c
```

### 7.2 Function Names

Function names must follow this format:

```text
module_action
```

Examples:

```c
bsp_i2c_mem_read
bsp_i2c_mem_write
bsp_uart_transmit
bsp_adc_read_raw
max30102_read_part_id
mq135_read_raw
air_quality_service_update
app_main_loop
```

### 7.3 Type Names

Type names must use the `_t` suffix.

Examples:

```c
bsp_i2c_status_t
max30102_status_t
mq135_status_t
air_quality_data_t
app_state_t
```

### 7.4 Macro Names

Macros must be uppercase.

Examples:

```c
BSP_I2C_TIMEOUT_MS
MAX30102_REG_PART_ID
MQ135_ADC_MAX_VALUE
APP_SENSOR_UPDATE_INTERVAL_MS
```

### 7.5 Local Variables

Local variables must be lowercase and use underscores.

Examples:

```c
chip_id
raw_value
timeout_ms
line_buffer
```

### 7.6 Global and Static Variables

Avoid global variables as much as possible.

If global or static file-scope variables are necessary:

- Use `g_` for global variables
- Use `s_` for static file-scope variables

Examples:

```c
g_health_data
s_oled_buffer
s_app_state
```

---

## 8. Header File Rules

Each `.h` file must contain:

1. Include guard
2. Minimal includes
3. Required macros
4. Public enum / struct / typedef if needed
5. Public function prototypes only

### 8.1 Header Guard Format

Example:

```c
#ifndef BSP_I2C_H
#define BSP_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* Public includes */

/* Public defines */

/* Public typedefs */

/* Public function prototypes */

#ifdef __cplusplus
}
#endif

#endif /* BSP_I2C_H */
```

### 8.2 No Function Body in Header

Do not put normal function bodies inside `.h` files.

Only use `static inline` if it is truly necessary and justified.

---

## 9. Source File Rules

Each `.c` file should follow this layout:

```c
/* Includes */

/* Private defines */

/* Private typedef */

/* Private variables */

/* Private function prototypes */

/* Public function definitions */

/* Private function definitions */
```

### 9.1 Private Functions

If a function is only used inside one `.c` file, it must be declared as `static`.

Example:

```c
static bsp_i2c_status_t bsp_i2c_from_hal_status(HAL_StatusTypeDef hal_status);
```

### 9.2 Public Functions

Public functions must not use `static`.

Public functions must be declared in the corresponding `.h` file.

---

## 10. Comment Rules

### 10.1 File Header

Each file must have a short header.

Example:

```c
/**
 * @file bsp_i2c.c
 * @brief BSP wrapper for STM32 HAL I2C functions.
 * @version 1.0.0
 */
```

### 10.2 Public Function Comment

Each public function must have a comment containing:

- Function purpose
- Input parameters
- Output parameters
- Return value

Example:

```c
/**
 * @brief Read memory data from an I2C device.
 * @param[in] dev_addr 7-bit device address shifted left by 1 if required by HAL usage.
 * @param[in] mem_addr Memory/register address.
 * @param[out] p_data Pointer to receive buffer.
 * @param[in] size Number of bytes to read.
 * @return bsp_i2c_status_t
 */
```

### 10.3 Avoid Useless Comments

Do not write comments that repeat obvious code.

Wrong:

```c
i++; /* Increase i */
```

Comment only where it helps explain intent, hardware behavior, register meaning, timing, state machine logic, or non-obvious decisions.

---

## 11. Code Format Rules

### 11.1 Always Use Braces

Wrong:

```c
if (x)
    y++;
```

Correct:

```c
if (x)
{
    y++;
}
```

### 11.2 Use Clear Spacing

Correct:

```c
if (hi2c == NULL || p_data == NULL)
{
    return BSP_I2C_ERROR;
}
```

Wrong:

```c
if(hi2c==NULL||p_data==NULL)
{
    return BSP_I2C_ERROR;
}
```

### 11.3 Brace Style

Use this brace style consistently:

```c
if (condition)
{
    do_something();
}
else
{
    do_other_thing();
}
```

### 11.4 Line Length

Avoid overly long lines when possible.

If a line becomes too long, split it clearly.

---

## 12. Error Handling Rules

Every function that can fail must check errors.

Always check:

- `NULL` pointer
- `size == 0U`
- invalid enum value
- invalid handle
- invalid configuration
- HAL return value
- BSP return value
- DeviceDriver return value

### 12.1 Do Not Ignore Return Values

Wrong:

```c
bsp_i2c_mem_read(...);
return MAX30102_OK;
```

Correct:

```c
status = bsp_i2c_mem_read(...);
if (status != BSP_I2C_OK)
{
    return MAX30102_ERROR;
}

return MAX30102_OK;
```

### 12.2 Example NULL and Size Check

```c
if (p_data == NULL || size == 0U)
{
    return BSP_I2C_ERROR;
}
```

---

## 13. Global Variable Rules

Global variables must be avoided as much as possible.

Only use global/static variables when truly needed, such as:

- latest sensor data
- display buffer
- hardware handle reference
- internal module state

Do not allow many files to modify the same global variable directly.

Prefer:

- static file-scope variables
- getter functions
- setter functions
- service-owned data structures

---

## 14. Module Responsibility Rules

Each module must have one main responsibility.

Correct:

```text
bsp_i2c          -> only handles I2C access at BSP level
bsp_uart         -> only handles UART access at BSP level
max30102         -> only controls MAX30102
mq135            -> only controls MQ135 ADC reading/conversion at driver level
air_quality_service -> processes MQ135 data into clean air-quality data
health_service   -> processes MAX30102 data into clean health data
app_main         -> coordinates application flow
```

Wrong:

```text
max30102.c reads sensor data and sends Bluetooth data
bsp_i2c.c reads I2C and calculates heart rate
main.c handles MAX30102 registers directly
mq135.c updates OLED screen directly
```

---

## 15. main.c Rules

`main.c` must stay as clean as possible.

It should only contain:

- HAL initialization
- System clock configuration
- CubeMX peripheral initialization
- `App_Init()`
- `App_Loop()` inside the main while loop

Expected style:

```c
int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_I2C1_Init();
    MX_USART1_UART_Init();
    MX_ADC1_Init();

    App_Init();

    while (1)
    {
        App_Loop();
    }
}
```

Do not put these directly in `main.c`:

- sensor register logic
- OLED display logic
- Bluetooth formatting logic
- alert algorithm
- data filtering
- large state machine implementation
- direct HAL reads/writes for application behavior

---

## 16. STM32CubeMX Generated Code Rules

Do not freely modify CubeMX generated sections.

Rules:

- Do not edit `.ioc` unless explicitly requested.
- Do not modify generated initialization functions unless explicitly requested.
- If editing generated files, prefer `USER CODE BEGIN` / `USER CODE END` blocks.
- Do not remove CubeMX comments or generated markers.
- Do not rename HAL handles without approval.

Examples of HAL handles that may exist depending on CubeMX configuration:

```c
extern I2C_HandleTypeDef hi2c1;
extern UART_HandleTypeDef huart1;
extern ADC_HandleTypeDef hadc1;
```

Do not invent a handle if the project does not define it.

If a handle is needed but not confirmed, ask or clearly state the assumption.

---

## 17. Current Hardware Context

Main target board:

- STM32F103C8T6 Bluepill

Known modules:

- SSD1306 OLED via I2C
- MAX30102 via I2C
- MQ135 via ADC
- HC-05 Bluetooth via UART
- RGB LED via GPIO/PWM
- Buzzer via GPIO/PWM
- Button 1
- Button 2
- Touch input

Known pin mapping:

```text
PA0  -> MQ135 ADC
PA1  -> RGB channel 1
PA2  -> RGB channel 2
PA3  -> RGB channel 3
PA8  -> Buzzer
PA9  -> UART TX to HC-05 RX
PA10 -> UART RX from HC-05 TX
PB6  -> I2C SCL
PB7  -> I2C SDA
PB12 -> Button 1
PB13 -> Button 2
PB14 -> Touch input
```

Do not change pin mapping unless the user explicitly requests it.

---

## 18. Build Rules

Code added to the project must be likely to build.

When adding new files, check or remind the user to check:

- `.c` files are not excluded from build
- include paths are configured correctly
- header files have include guards
- function prototypes match function definitions
- required HAL handles exist
- no circular includes
- no missing typedefs
- no missing macros
- no direct HAL calls outside BSP

Common include paths that may need to be added in STM32CubeIDE:

```text
../App/Inc
../Assets/Inc
../BSP/Inc
../DeviceDrivers/Inc
```

---

## 19. Debug Rules

Debug from small to large and from low layer to high layer.

Recommended debug order:

```text
1. Hardware wiring
2. CubeMX peripheral configuration
3. HAL handle existence
4. BSP function
5. DeviceDrivers function
6. Assets/Service processing
7. App flow
8. main.c integration
```

Examples:

- If I2C communication fails, debug `bsp_i2c` before debugging `max30102`.
- If MAX30102 PART ID cannot be read, debug wiring, I2C address, CubeMX I2C config, then BSP, then driver.
- If display data is wrong, debug service formatting before changing OLED driver.
- If app flow is wrong, check service update intervals and state machine.

Do not write five new files at once and only build at the end.

Preferred workflow:

```text
1. Create skeleton
2. Build
3. Test BSP
4. Test DeviceDriver
5. Test Service
6. Integrate with App
```

---

## 20. AI Agent Working Rules

AI may assist with:

- explaining code
- generating skeletons
- writing small modules
- reviewing code
- suggesting debug checklists
- refactoring code according to rules

AI must not:

- replace understanding of the project
- paste unexplained code
- ignore the layer architecture
- invent hardware configuration
- change pin mapping without permission
- rewrite the whole project without permission
- modify unrelated modules
- hide assumptions
- edit `.ioc`, `main.c`, or generated code without explicit permission

Each generated module must include:

- `.h` file
- `.c` file
- clear public API
- basic comments
- error handling
- build notes
- minimal test instructions

---

## 21. Agent Permission Rules

Before changing files, the agent should state:

- Files to create
- Files to edit
- Files not to touch
- Assumptions

The agent must ask for approval before:

- deleting files
- renaming files
- editing `.ioc`
- editing generated CubeMX initialization code
- making large architecture changes
- modifying `main.c` beyond adding `App_Init()` and `App_Loop()`
- changing pin mapping
- changing peripheral configuration
- changing public APIs used by other modules

---

## 22. Standard Output Format for Code Generation

When generating or modifying code, use this response format:

```text
1. Layer
2. Module responsibility
3. Files created/edited
4. Assumptions
5. Header file content
6. Source file content
7. Integration notes
8. Minimal test steps
9. Build/debug checklist
```

If modifying existing files, also include:

```text
10. Summary of changes
11. Possible risks
```

---

## 23. Standard Self-Review Checklist

Before finalizing any code, the agent must self-check:

```text
1. Is the module in the correct layer?
2. Are all includes legal?
3. Is there any direct HAL call outside BSP?
4. Is there any reverse layer dependency?
5. Is main.c kept clean?
6. Are file names correct?
7. Are function names in module_action format?
8. Do typedef names end with _t?
9. Are macros uppercase?
10. Does every header have an include guard?
11. Are public functions commented?
12. Are private functions static?
13. Are NULL pointers checked?
14. Is size == 0U checked where needed?
15. Are HAL/BSP/Driver return values checked?
16. Are global variables minimized?
17. Is each module limited to one responsibility?
18. Are include paths likely correct?
19. Can the code likely build?
20. Are minimal test steps provided?
```

If any item fails, fix the code before providing the final answer.

---

## 24. Skill: Create BSP Module

Use this skill when creating a BSP module.

Layer:

```text
BSP
```

Allowed to call:

```text
HAL
```

Not allowed to call:

```text
DeviceDrivers
Assets/Service
App
```

Required output:

- `bsp_xxx.h`
- `bsp_xxx.c`
- status enum
- public APIs
- HAL status mapping if needed
- error checking
- comments for public functions
- minimal test instructions

Common BSP modules:

```text
bsp_i2c
bsp_uart
bsp_adc
bsp_gpio
bsp_time
bsp_pwm
```

Example APIs:

```c
bsp_i2c_mem_read
bsp_i2c_mem_write
bsp_i2c_is_device_ready
bsp_uart_transmit
bsp_uart_receive
bsp_adc_read_raw
bsp_gpio_write
bsp_gpio_read
bsp_time_get_tick
bsp_time_delay_ms
```

---

## 25. Skill: Create Device Driver

Use this skill when creating a driver for a specific hardware device.

Layer:

```text
DeviceDrivers
```

Allowed to call:

```text
BSP
```

Not allowed to call:

```text
HAL directly
Assets/Service
App
```

Required output:

- `device_name.h`
- `device_name.c`
- status enum
- device config struct if needed
- init function
- read/write/control APIs
- register macros if needed
- error checking for BSP return values
- minimal test instructions

Rules:

- Do not implement high-level application logic here.
- Do not send Bluetooth data from a sensor driver.
- Do not update OLED from a sensor driver.
- Do not call App functions.
- Do not call HAL directly.

Common driver APIs:

```c
device_init
device_read_id
device_read_raw
device_write_config
device_start
device_stop
device_reset
```

---

## 26. Skill: Create Assets/Service Module

Use this skill when creating a service module.

Layer:

```text
Assets/Service
```

Allowed to call:

```text
DeviceDrivers
```

Not allowed to call:

```text
HAL directly
BSP directly unless approved
App
```

Required output:

- `service_name.h`
- `service_name.c`
- clean data struct
- init function
- update/process function
- getter function if needed
- error handling
- minimal test instructions

Common service APIs:

```c
service_init
service_update
service_get_data
service_reset
```

Example service responsibilities:

```text
air_quality_service -> convert MQ135 raw data into clean air quality data
health_service      -> process MAX30102 data
display_service     -> format data for OLED
bluetooth_service   -> format/send clean data to HC-05
alert_service       -> decide buzzer/RGB behavior from clean status
```

---

## 27. Skill: Create App Layer

Use this skill when creating or editing the App layer.

Layer:

```text
App
```

Allowed to call:

```text
Assets/Service
```

Not allowed to call:

```text
HAL directly
BSP directly
DeviceDrivers directly unless explicitly approved
```

Required output:

- `app_main.h`
- `app_main.c`
- `App_Init()`
- `App_Loop()`
- non-blocking timing pattern when possible
- clear service coordination

Rules:

- `App_Init()` initializes services.
- `App_Loop()` calls service update functions.
- Avoid `HAL_Delay()` inside `App_Loop()`.
- Do not write register-level code.
- Do not process raw sensor data directly.

---

## 28. Skill: Review STM32 Code

Use this skill when reviewing code.

Review checklist:

```text
1. Correct layer?
2. Any forbidden include?
3. Any direct HAL call from App/Service/Driver?
4. Any logic wrongly placed in main.c?
5. Naming convention correct?
6. Header guard exists?
7. Public functions have comments?
8. Private functions are static?
9. Error handling exists?
10. HAL/BSP/Driver return values checked?
11. Global variables minimized?
12. Module responsibility clear?
13. Format consistent?
14. Build risk?
15. Minimal debug/test path?
```

Output format:

```text
1. Critical issues
2. Architecture violations
3. Naming/style issues
4. Error handling issues
5. Build risks
6. Suggested fixes
7. Corrected code if requested
```

---

## 29. Skill: Layered Debug

Use this skill when debugging STM32 issues.

Debug order:

```text
1. Hardware wiring
2. CubeMX config
3. HAL handle
4. BSP
5. Device Driver
6. Assets/Service
7. App
8. main.c integration
```

Output format:

```text
1. Most likely layer of failure
2. Possible causes
3. Step-by-step checks
4. Minimal test code if needed
5. Recommended fix
```

Rules:

- Do not guess wildly.
- Do not skip low-level checks.
- Test from small to large.
- Fix the layer where the bug belongs.

---

## 30. Prompt Templates for Future Agent Use

### 30.1 Create BSP Module

```text
Read AGENTS.md first.

Create a BSP module for [PERIPHERAL].

Requirements:
- Layer: BSP
- Create BSP/Inc/bsp_[peripheral].h
- Create BSP/Src/bsp_[peripheral].c
- BSP may call HAL
- Do not call DeviceDrivers, Assets/Service, or App
- Add status enum
- Add error handling
- Add public function comments
- Provide minimal test steps
- Do not edit main.c unless explicitly required
```

### 30.2 Create Device Driver

```text
Read AGENTS.md first.

Create a device driver for [DEVICE_NAME].

Requirements:
- Layer: DeviceDrivers
- Create DeviceDrivers/Inc/[device_name].h
- Create DeviceDrivers/Src/[device_name].c
- Driver must call BSP only
- Driver must not call HAL directly
- Do not include App or Service headers
- Add init/read/write/control APIs
- Add status enum
- Add error handling
- Provide minimal test steps
```

### 30.3 Create Service Module

```text
Read AGENTS.md first.

Create a service module for [SERVICE_NAME].

Requirements:
- Layer: Assets/Service
- Create Assets/Inc/[service_name].h
- Create Assets/Src/[service_name].c
- Service may call DeviceDrivers
- Service must not call HAL directly
- Service must not call App
- Add clean data struct
- Add init/update/get APIs
- Add error handling
- Provide minimal test steps
```

### 30.4 Create App Layer

```text
Read AGENTS.md first.

Create or update App layer.

Requirements:
- Layer: App
- Use App/Inc/app_main.h
- Use App/Src/app_main.c
- Add App_Init()
- Add App_Loop()
- App may call Services only
- App must not call HAL, BSP, or DeviceDrivers directly
- Keep main.c clean
- Avoid blocking delays in App_Loop
```

### 30.5 Review Code

```text
Read AGENTS.md first.

Review the selected code according to project rules.

Focus on:
- Layer violations
- Direct HAL calls outside BSP
- Wrong includes
- Dirty main.c
- Missing error handling
- Naming convention
- Missing comments
- Build risks

Return Pass/Fail table and suggested fixes.
```

---

## 31. Final Rule

When unsure, prefer safe, small, buildable changes.

Do not make large automatic changes.

Do not hide assumptions.

Do not violate the architecture.

Always keep the code clear enough that another team member can understand, test, and maintain it.
