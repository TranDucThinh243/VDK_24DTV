# Integrate STM32 Module Safely Skill

## Purpose

Use this skill when integrating a finished BSP, Device Driver, or Service module into the larger STM32 project.

Integration must be incremental and buildable at each step.

## Integration Order

Use this order:

```text
1. Confirm module files exist
2. Confirm include path
3. Confirm .c file is included in build
4. Compile without App integration
5. Add minimal test call at the correct layer
6. Compile again
7. Add service integration
8. Compile again
9. Add App integration
10. Compile again
11. Remove temporary test code if needed
```

## Layer Rules

Integration must respect:

```text
App -> Assets/Service -> DeviceDrivers -> BSP -> HAL
```

Do not bypass layers to make testing quick unless the user explicitly requests a temporary test and the code is clearly marked temporary.

## CubeMX Generated Code Rules

When touching CubeMX generated files:

- Prefer editing only inside USER CODE blocks.
- Do not move generated init functions.
- Do not rename HAL handles unless CubeMX config is changed intentionally.
- Do not edit `.ioc` unless requested.

## main.c Rules

`main.c` should only call:

```c
App_Init();

while (1)
{
    App_Loop();
}
```

Temporary test code in `main.c` is allowed only if:

1. The user asks for it.
2. It is placed inside USER CODE blocks.
3. It is clearly marked as temporary.
4. There is a note to remove it after testing.

## Output Format

When using this skill, respond with:

```markdown
## Integration Target

## Files To Check

## Include Paths Needed

## Build Steps

## Minimal Test Step

## App Integration Step

## Rollback Plan

## Risks
```

## Safety Rules

Do not integrate multiple unrelated modules at once.

Do not hide build risks.

Do not modify unrelated files.
