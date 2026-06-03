# Review STM32 Code Skill

## Purpose

Use this skill when reviewing STM32 project code against the team's coding rules.

The goal is to find architecture, style, build, and maintainability issues before merging or continuing integration.

## Review Checklist

Check these items in order:

1. Correct layer placement
2. Forbidden dependency or include
3. Direct HAL call outside BSP
4. DeviceDrivers calling App or Service
5. BSP including DeviceDrivers/App
6. App calling BSP/HAL/DeviceDrivers directly
7. `main.c` containing too much logic
8. Missing `.h` / `.c` pair
9. Missing include guard
10. Wrong naming convention
11. Missing public function comments
12. Private functions missing `static`
13. Missing NULL checks
14. Missing `size == 0U` checks where relevant
15. Ignored HAL/BSP/Driver return values
16. Excessive or unclear global variables
17. Mixed module responsibilities
18. Possible circular include
19. Missing include path/build risk
20. Unclear test path

## Output Format

Return review results in this structure:

```markdown
## Critical Issues

## Architecture Violations

## Error Handling Issues

## Naming / Style Issues

## Build Risks

## Suggested Fixes

## Minimal Patch Plan
```

For each issue, include:

```text
- File:
- Problem:
- Why it matters:
- Suggested fix:
```

## Pass/Fail Table

When the user asks for strict review, include this table:

```markdown
| Rule | Pass/Fail | Evidence | Fix |
|---|---|---|---|
```

## Safety Rules

Do not rewrite all code unless the user explicitly asks.

Prefer explaining the problem first, then provide a small patch.

Do not change project architecture casually.
