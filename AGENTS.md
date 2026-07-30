# Development Rules

This document defines how changes should be made in this repository.

## Change Process

Make changes incrementally.

Each change should:

- Modify the smallest number of files possible.
- Have one clear purpose.
- Be built and tested before starting another change.
- Avoid unrelated cleanup or refactoring.

Do not combine architectural changes with feature implementation unless the change requires it.

## Architecture Stability

The current architecture should be treated as the baseline.

Do not move responsibilities between layers without first explaining the reason and trade-offs.

The current direction is:

```text
runtime
    |
    v
application
    |
    v
engine systems
```

The goal is to keep the engine reusable while allowing projects to contain their own game logic.

File Changes

When modifying code:

Always state the file path being changed.
Explain what problem the change solves.
Avoid changing many files at once.
Prefer adding small pieces of functionality over rewriting existing systems.

Related .hpp and .cpp files may be changed together when they represent the same class.

### C++ Style

Keep class interfaces and implementations separated.

Headers should contain declarations.

Source files should contain implementation details.

Prefer controlled access through functions instead of exposing internal state.

Example:

```c++
object.stop();
```

is prefered over:

```c++
object.running = false;
```

### Testing Changes

After each change:

1. Configure if required.
2. Build the project.
3. Run the affected executable if possible.

Do not continue if the previous change does not build.

### Avoid Premature Design Changes

Do not introduce systems, abstractions, or frameworks before they are needed.

The project should grow from working requirements rather than trying to predict the final engine design.

### Current Development Focus

The project is currently building the engine foundation.

Priorities should be:

- Stable engine lifecycle.
- Core systems.
- Runtime integration.
- Rendering foundation.
- Input handling.
- Scripting support.

Large architectural redesigns should only happen when a concrete limitation is discovered.
