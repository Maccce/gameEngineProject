# Development Rules

This document defines how changes should be made in this repository.

## Change Process

Make changes incrementally.

Each change should:

- Modify the smallest number of files possible.
- Have one clear purpose.
- Build before continuing.
- Avoid unrelated cleanup or refactoring.

Do not combine feature work with architectural changes unless the feature requires it.

## Architecture

The project architecture is documented in:

    docs/architecture.md

Treat that document as the architectural source of truth.

The current architecture may be partially implemented.

Do not infer architectural problems from missing features or missing systems.

A system that does not exist yet is not automatically a reason to introduce:
- new layers,
- new abstractions,
- new ownership boundaries,
- architectural redesigns.

Do not propose architectural changes unless:

- the current implementation creates a concrete limitation,
- the limitation cannot reasonably be solved within the existing architecture.

When suggesting an architectural change, always explain:

- the current limitation,
- why the limitation exists,
- why the proposed solution improves the project,
- the trade-offs involved.

Do not move responsibilities between:
- runtime,
- application,
- projects,
- engine,
- systems,

without first explaining why the ownership boundary is incorrect.

## File Changes

Before making a change, identify:

- the files being modified,
- the problem being solved,
- why those files are the correct place for the change.

Avoid changing unrelated files.

Related `.hpp` and `.cpp` files may be changed together.

## C++ Style

Keep declarations in headers.

Keep implementation in source files.

Prefer encapsulation over exposing mutable state.

Prefer:

    object.stop();

over:

    object.running = false;

## Development Style

Prefer implementing the existing architecture over redesigning it.

Do not add extension points until they are required by the implementation.

Choose the simplest solution that satisfies the current requirement.

Missing functionality should normally be implemented inside the existing ownership boundaries before creating new layers.

## Validation

After each logical change:

1. Configure if necessary.
2. Build the project.
3. Run the affected executable when possible.

Do not continue if the previous change does not build.
