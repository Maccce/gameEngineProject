# Engine Architecture

This document defines the architecture, responsibilities, dependencies, and ownership boundaries of `gameEngineProject`.

The engine provides reusable functionality. Projects provide application-specific behaviour.

## Design Goals

- Keep the engine independent from individual projects.
- Keep runtime startup minimal and generic.
- Separate application lifecycle management from engine implementation.
- Support dynamically loaded projects through a versioned C API.
- Keep project-specific implementations inside their respective projects.
- Support runtime applications, editor tooling, and development tools.

## Architecture Layers

```text
runtime/
    Generic executable entry point

application/
    Application lifecycle and project management

engine/
    Reusable engine systems and their lifecycle

projects/
    Project implementations and shared libraries

editor/
    Separate editor application

tools/
    Development and asset pipeline tools
```

### Runtime

The runtime is the executable entry point.

Responsibilities:
- Construct the application.
- Start the application.
- Return its exit code.

The runtime does not implement engine systems, project discovery, project loading, or gameplay logic. It depends on the application framework rather than on concrete projects.

### Application

The application framework coordinates the application lifecycle and its interaction with the engine and selected project.

Responsibilities:
- Start and stop the engine.
- Define the project C API contract.
- Discover and select projects.
- Load the selected project shared library.
- Coordinate project initialization, updates, and shutdown.
- Ensure the project is destroyed before its shared library is unloaded.

The application contains no concrete project implementation.

#### Project C API

`application/project_api.h` defines the C-compatible interface between the application and project libraries.

The interface provides:
- `AppProjectHandle`: an opaque handle to a project instance.
- `AppProjectAPI`: a versioned function table for creating, initializing, updating, shutting down, and destroying a project.
- `AppGetProjectAPI`: the function-pointer type used to retrieve the API.
- `getProjectAPI`: the entry point exported by each project library.

The API version is defined by `APP_PROJECT_API_VERSION`. The application requests a supported version and validates the returned version and required function pointers before creating a project.

The function table contains these operations:

```c
AppProjectHandle* (*create_project)(void);

void (*initialize_project)(AppProjectHandle* project);

void (*update_project)(
    AppProjectHandle* project,
    double delta_time
);

void (*shutdown_project)(AppProjectHandle* project);

void (*destroy_project)(AppProjectHandle* project);
```

The project library defines the concrete `AppProjectHandle` type privately. The application treats it as opaque and interacts with it only through the API functions.

The interface uses C linkage and C-compatible data types at the boundary, avoiding the need for a shared C++ virtual interface between the application and project library. Project implementations may still use C++ internally.

### Project Registry

Location:

```text
application/
├── ProjectRegistry.hpp
├── ProjectRegistry.cpp
└── ProjectInfo.hpp
```

The registry discovers projects in a supplied directory.

Responsibilities:
- Find project directories containing `project.json`.
- Read metadata such as name, description, and version.
- Store project information, including the directory path.
- Expose the available projects to the application.

The registry does not load shared libraries or create project instances.

Example metadata:

```json
{
  "name": "Example Game",
  "description": "An example game project.",
  "version": "0.1.0"
}
```

The application uses the resulting project list to present a selection.

#### Project Loader

Location:

```text
application/
├── ProjectLoader.hpp
├── ProjectLoader.cpp
├── SharedLibrary.hpp
├── SharedLibraryLinux.cpp
└── project_api.h
```

The project loader receives the selected `ProjectInfo` and loads the corresponding shared library.

Responsibilities:
- Resolve the project's library path.
- Load the shared library.
- Find the exported `getProjectAPI` entry point.
- Request and validate the project API version.
- Create a project instance through the API.
- Return a `LoadedProject` owner that manages the project handle and shared library.

The loader uses the project directory name to derive the library name in the current implementation. The current library path convention is `build/lib`.

The platform-specific shared-library implementation is currently Linux-based.

### Loaded Project Ownership

`LoadedProject` owns the loaded shared library, the project handle, and a pointer to the project's API function table.

The API pointer and function pointers remain valid only while the shared library is loaded.

Required destruction order:

```text
Shut down project if initialized
        |
        v
Destroy project instance through the C API
        |
        v
Unload shared library
```

The project library supplies the function that destroys its own project instance. The application must not unload the shared library before destruction completes.

There is no separate C++ project module object in the current design.

### Engine

The engine provides reusable functionality and does not depend on projects or the application framework.

Responsibilities:
- Provide reusable systems.
- Coordinate engine initialization and shutdown.
- Expose engine functionality through public interfaces.

The application starts and stops the engine, but does not implement its systems.

#### Engine Lifecycle

```text
Application
    |
    +-- Initialize Engine
    |       |
    |       v
    |     Initialize systems
    |
    +-- Run application and project
    |
    +-- Shut down Engine
            |
            v
        Shut down systems
```

The exact systems participating in the lifecycle depend on the engine implementation.

#### Engine Modules and Systems

Engine modules describe the organization of engine code. Systems provide reusable areas of functionality. These concepts are related but are not necessarily one-to-one.

Potential engine modules include:

```text
engine/
├── audio/
├── core/
├── debugging/
├── ecs/
├── events/
├── physics/
├── platform/
├── renderer/
├── resources/
└── scripting/
```

Potential systems include time, input, rendering, physics, audio, resources, events, and ECS functionality. Some of these are planned rather than fully implemented.

Systems:
- Belong to the engine.
- Provide reusable functionality.
- Are independent of specific projects.
- Are initialized, updated, or shut down according to the engine's lifecycle design.

They do not contain project-specific gameplay rules.

### Projects

Projects are games or other applications built using the engine.

A project owns its:
- Project-specific behaviour and gameplay.
- Concrete project implementation.
- C API entry point and function table.
- Project assets and configuration.
- Project-specific systems and data.

Example:

```text
projects/
└── ExampleGame/
    ├── project.json
    ├── CMakeLists.txt
    └── src/
        ├── Game.hpp
        ├── Game.cpp
        └── GameModule.cpp
```

`Game.cpp` and `Game.hpp` contain the concrete game implementation. `GameModule.cpp` defines the opaque project handle, implements the C API callbacks, and exports `getProjectAPI`.

The project library creates and destroys its concrete project instance through these callbacks. The application does not include the concrete project's headers or refer to its class names.

Projects use engine functionality through the engine's public API. They do not own the engine's system implementations or lifecycle.

### Editor

The editor is intended to be a separate application built using the engine.

Potential responsibilities include:
- Scene editing.
- Entity inspection.
- Asset management.
- Debug visualization.
- Development workflows.

Editor functionality remains separate from the runtime application.

### Tools

Tools support development workflows independently of the runtime.

Examples include:
- Asset processing.
- Project generation.
- Shader compilation.
- Packaging.

## Dependencies and Relationships

In the following diagrams, an arrow means that the source depends on the destination. It does not, by itself, indicate ownership or execution order.

### Dependency Relationships

```text
Runtime
    |
    v
Application
    |
    +------------------+
    |                  |
    v                  v
Project C API         Engine
    ^
    |
Project Library
    |
    v
Engine
```

The project library implements the C API contract, and its concrete project implementation uses engine functionality.

At a high level:

```text
Runtime -> Application
Application -> Engine
Application -> Project C API
Application -> Project Loader
Project Loader -> Shared Library API
Project Library -> Project C API
Project Implementation -> Engine
```

The project implementation and C API entry point are built into the project's shared library. The application does not depend on a concrete project's target or class.

### Project Loading Flow

This sequence describes runtime execution, not dependency direction:

```text
Runtime starts Application
          |
          v
Application starts Engine
          |
          v
Application discovers projects
          |
          v
User selects a project
          |
          v
Project Loader loads shared library
          |
          v
Loader resolves getProjectAPI
          |
          v
Loader requests and validates API version
          |
          v
API creates project instance
          |
          v
Application runs project lifecycle
          |
          v
Project is shut down
          |
          v
Project instance is destroyed through the C API
          |
          v
Shared library is unloaded
```

### Lifecycle Coordination

The application coordinates both engine and project lifecycles:

```text
Application
    |
    +-- Start Engine
    |
    +-- Load Project
    |
    +-- Initialize Project
    |
    +-- Update Project
    |
    +-- Shut down Project
    |
    +-- Destroy Project and Unload Library
    |
    +-- Stop Engine
```

The engine owns its systems. The project library owns the concrete project implementation and its creation and destruction functions. The loaded-project owner keeps the shared library loaded for the duration of the project instance.

## Architectural Rules

1. The engine does not depend on projects.
2. The runtime starts the application and remains generic.
3. The application owns application lifecycle coordination.
4. The application defines the versioned project C API contract.
5. Projects implement that API inside their own shared libraries.
6. The application discovers, selects, and loads projects without naming concrete project classes.
7. Project libraries create and destroy their own project implementations through the C API.
8. Projects use engine functionality through the engine's public API.
9. The engine manages its own systems and their lifecycle.
10. A project must be destroyed before its shared library is unloaded.
11. Editor and development tools remain separate from the runtime application.

These boundaries allow projects to be added or replaced without introducing project-specific dependencies into the engine or runtime.
