# Debug STM32 By Layer Skill

## Purpose

Use this skill when debugging STM32 code, integration problems, build errors, runtime issues, or hardware communication failures.

The project must be debugged from low layer to high layer.

## Debug Order

Always debug in this order:

```text
1. Hardware wiring
2. Power and ground
3. CubeMX peripheral configuration
4. HAL handle availability
5. BSP function
6. Device Driver function
7. Assets/Service processing
8. App flow
9. main.c integration
```

Do not jump to App-level fixes before proving BSP and Driver work.

## Questions To Inspect

For hardware/peripheral issues:

```text
- Which peripheral is involved?
- Which pins are used?
- Is the module powered correctly?
- Is GND common?
- Are pull-up resistors needed?
- Is the address/register correct?
```

For build errors:

```text
- Which file fails?
- Is the header included correctly?
- Is include path configured?
- Is the .c file excluded from build?
- Are prototypes and implementations matching?
- Are HAL handles declared in the right place?
```

For architecture issues:

```text
- Which layer owns the failing logic?
- Is HAL called outside BSP?
- Is App calling Driver directly?
- Is there a circular include?
```

## Output Format

Return debugging guidance in this structure:

```markdown
## Symptom

## Most Likely Layer

## Likely Causes

## Step-by-step Debug Plan

## Minimal Test Code or Test Action

## Fix Recommendation

## What Not To Change Yet
```

## Minimal Test Strategy

For each module, test from the smallest unit:

```text
BSP I2C: scan/read device ready
MAX30102 driver: read PART ID
BSP ADC: read raw ADC value
MQ135 driver: read raw ADC through driver
SSD1306 driver: clear screen and draw test text
HC-05 driver: send fixed text over UART
Service: process fake or known driver data
App: call service update by interval
```

## Safety Rules

Do not propose rewriting the full project for one bug.

Do not modify `.ioc` unless the issue is truly peripheral configuration.

Do not modify `main.c` unless integration is the confirmed problem.
