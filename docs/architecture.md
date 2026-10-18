# Engine Architecture

This document defines the architecture, responsibilities, dependencies, and ownership boundaries of `gameEngineProject`.

The engine provides reusable functionality. Projects provide application-specific behaviour.

## Design Goals

- Keep the engine independent from individual projects.
- Keep runtime startup minimal and generic.
- Separate application lifecycle management from engine implementation.
- Support dynamically loaded projects through a common interface.
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
    Project implementations and project modules

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
- Define the project interface.
- Discover and select projects.
- Load the selected project module.
- Coordinate project initialization, updates, and shutdown.
- Ensure the project is destroyed before its module and shared library are released.

The application contains no concrete project implementation.

#### Project Interface

`App::Project` is the common interface implemented by projects. It defines lifecycle operations such as initialization, updating, and shutdown.

Projects implement this interface within their own source code.

The application can therefore operate on a project without knowing its concrete class.

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
└── SharedLibraryLinux.cpp
```

The project loader receives the selected `ProjectInfo` and loads the corresponding shared library.

Responsibilities:
- Resolve the project's library path.
- Load the shared library.
- Find the generic `createProjectModule` entry point.
- Create a project module.
- Create a project instance through that module.
- Return an owner that manages the loaded project, module, and library.

The loader uses the project directory name to derive the library name in the current implementation. The current library path convention is `build/lib`.

The platform-specific shared-library implementation is currently Linux-based.

#### Project Module

`App::ProjectModule` defines the interface used to create and destroy project instances.

Each project supplies its own module implementation and exports the entry point:

```cpp
extern "C"
App::ProjectModule* createProjectModule();
```

The entry point has C linkage so the loader can find its symbol by name. The returned interface is still a C++ interface; this design assumes a compatible compiler and C++ ABI between the runtime and project library.

The module interface includes operations to create and destroy a project:

```cpp
virtual Project* createProject() = 0;
virtual void destroyProject(Project* project) = 0;
```

The concrete module and project classes remain inside the project. The application does not include their headers or refer to their names.

### Loaded Project Ownership

The loaded-project owner keeps the shared library loaded for as long as the project and module may execute code from it.

Required destruction order:

```text
Destroy project instance
        |
        v
Destroy project module
        |
        v
Unload shared library
```

The module destroys the project instance it created. The application must not unload the shared library before these destruction operations complete.

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
- Concrete implementation of `App::Project`.
- Project module implementation.
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
        ├── GameModule.hpp
        └── GameModule.cpp
```

The project module creates and destroys the concrete project implementation. The project uses engine functionality through the engine's public API.

Projects do not own the engine's system implementations or lifecycle.

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
    +----------------+
    |                |
    v                v
Project Interface   Engine
    ^
    |
Project Implementation
    |
    v
Engine
```

The project module implements the application-defined module interface, and the project implementation implements the project interface.

At a high level:

```text
Runtime -> Application
Application -> Engine
Application -> Project Interface
Application -> Project Loader
Project Loader -> Shared Library API
Project Module -> Project Interface
Project Implementation -> Project Interface
Project Implementation -> Engine
```

The concrete project module and implementation are built into the project's shared library. The application does not depend on a concrete project's target or class.

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
Generic module entry point is called
          |
          v
Module creates project instance
          |
          v
Application runs project lifecycle
          |
          v
Project is destroyed
          |
          v
Module is destroyed
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
    +-- Unload Project
    |
    +-- Stop Engine
```

The engine owns its systems. The project module owns creation and destruction of the concrete project instance. The loaded-project owner keeps the module and its shared library alive for the duration of the project instance.

## Architectural Rules

1. The engine does not depend on projects.
2. The runtime starts the application and remains generic.
3. The application owns application lifecycle coordination.
4. The application defines the project and project-module interfaces.
5. Projects implement those interfaces inside their own source code.
6. The application discovers, selects, and loads projects without naming concrete project classes.
7. Project modules create and destroy their project implementations.
8. Projects use engine functionality through the engine's public API.
9. The engine manages its own systems and their lifecycle.
10. A project and its module must be destroyed before their shared library is unloaded.
11. Editor and development tools remain separate from the runtime application.

These boundaries allow projects to be added or replaced without introducing project-specific dependencies into the engine or runtime.
