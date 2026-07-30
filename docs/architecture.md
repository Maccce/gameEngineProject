# Engine Architecture

This document defines the architectural structure and ownership boundaries of the engine project.

The engine is designed as a reusable foundation that can support multiple games and applications.

## Design Goals

The engine should be:

- Reusable across multiple projects.
- Independent from specific games or applications.
- Modular.
- Cross-platform.
- Suitable for runtime applications, editor tooling, and development tools.

The engine provides reusable functionality.

Projects provide application-specific behaviour.

## Architecture Layers

The repository is divided into four main architectural areas:

```text
runtime/

    Starts the executable.


application/

    Hosts an engine application.
    Defines the project interface.


projects/

    Contains games and applications using the engine.


engine/

    Provides reusable engine functionality and systems.

editor/

    Separate application built using the engine.

tools/

    Applications supporting development workflows.
```

### Runtime

The runtime is the executable entry point.

Responsibilities:

- Start the process.
- Create the application environment.
- Start execution.

The runtime remains generic.

The runtime does not contain:

- gameplay logic,
- engine systems,
- project-specific code.

### Applications

The application layer provides the framework for running an engine-based application.

It manages the relationship between the runtime, project lifecycle, and engine startup.

Responsibilities:

- Manage application lifetime.
- Coordinate engine startup and shutdown.
- Own the project interface.
- Load and run a selected project.
- Provide the application environment required by projects.

The application layer does not contain:

- gameplay rules,
- project-specific behaviour,
- engine system implementations.

#### Application

The application is responsible for lifecycle and coordination layer.

Physical location:

application/

    Application.hpp
    Application.cpp

Responsibilities:

- Managing the lifetime of an engine-based application.
- Starting and shutting down the engine.
- Loading and running a project through the project interface.
- Coordinating the main application loop.
- Connecting runtime execution with projects and engine functionality.

Application does not contain:

- Gameplay logic.
- Project-specific behaviour.
- Engine system implementations.
- Rendering, physics, audio, or ECS functionality.

The Application layer is responsible for hosting a project and coordinating the
use of the engine, while projects provide application-specific behaviour and
the engine provides reusable systems.

#### Project Interface

The project interface defines the contract between the application layer and projects.

The application layer owns this interface because it defines how a project is hosted and executed.

Projects implement this interface.

Relationship:

```text
application/

    Project Interface
             ^
             |
             |
        Game Project
```

The project interface allows multiple projects to run using the same application framework.

#### Project Loader

The project loader is responsible for loading projects into the application.

Physical location:

application/

    ProjectLoader.hpp
    ProjectLoader.cpp

Responsibilities:

Locate projects.
Load projects.
Create project instances.
Provide the project instance to the application.

Project loading is an application responsibility.

The engine does not know how projects are discovered or loaded.

### Engine

The engine provides reusable functionality.

The engine is independent from:

- games,
- projects,
- runtime applications,
- editor applications.

The engine does not know which project is running.

The engine contains systems that provide reusable functionality.

Relationship:

```text
Engine
    |
    v
Systems
```

### Engine Modules

Engine modules are physical code organization.

They define where engine functionality is implemented.

Example:

```text
engine/
├── animation/
├── audio/
├── core/
├── debugging/
├── ecs/
├── events/
├── hotreload/
├── jobs/
├── networking/
├── physics/
├── platform/
├── reflection/
├── renderer/
├── resources/
└── scripting/
```

Modules provide reusable functionality.

A module does not necessarily represent exactly one runtime system.

### Systems

Systems are the modular building blocks of the engine.

Each system provides a specific area of engine functionality.

Examples:

```text
Engine
    |
    +-- Time System
    |
    +-- Input System
    |
    +-- Renderer System
    |
    +-- Physics System
    |
    +-- Audio System
    |
    +-- ECS Systems
    |
    +-- Resource System
    |
    +-- Event System
    |
    +-- Scripting System
    |
    +-- Networking System
```

Systems:

- belong to the engine.
- provide reusable functionality.
- are independent from specific projects.
- are coordinated by the engine.

Systems do not contain:

- gameplay rules,
- project-specific behaviour,
- game-specific objects.

Projects use engine systems through the engine API.

### Projects

Projects are applications built using the engine.

Examples:

- Games.
- Simulations.
- Other engine-based applications.

Projects contain:

- Gameplay logic.
- Project-specific systems.
- Scenes.
- Project assets.
- Project configuration.

Projects:

- implement the project interface.
- use engine functionality.
- provide project-specific behaviour.

Projects do not own engine systems.

### Editor

The editor is a separate application built using the engine.

Responsibilities:

- Scene editing.
- Entity inspection.
- Asset management.
- Debug tooling.
- Development workflows.

The editor remains separate from runtime applications.

### Tools

Tools support development workflows.

Examples:

- Asset processing.
- Project generation.
- Shader compilation.
- Packaging.

Tools are separate applications.

## Relationships

The architecture consists of two related flows.

### Application Execution Flow

```text
Runtime
    |
    v
Application
    |
    +----------------+
    |                |
    v                v
Project          Engine
Interface          |
    ^              v
    |            Systems
Game Project
```

### Project Usage Flow

```text
Game Project
    |
    +--> Project Interface
    |
    +--> Engine
            |
            v
          Systems
```

The diagrams describe relationships, not ownership hierarchy.

The important rules are:

- Runtime starts the application.
- Application hosts projects.
- Projects implement the project interface.
- Projects use the engine.
- Engine provides systems.
- Engine does not depend on projects.

### Dependency Rules

Allowed dependencies:

```text
Runtime
    |
    v
Application
    |
    v
Engine


Projects
    |
    +--> Application Project Interface
    |
    +--> Engine
```

The following dependencies are not allowed:

- Engine depending on projects.
- Engine depending on runtime.
- Engine depending on editor code.
- Engine depending on tools.
- Projects modifying engine ownership boundaries.

### Repository Ownership

The repository structure follows these responsibilities:

```text
engine/

    Reusable engine functionality.

    Contains:
        core/
        systems/
        renderer/
        physics/
        audio/
        ecs/
        resources/
        events/
        scripting/

application/

    Application framework.

    Contains:
        Application
        ProjectInterface

runtime/

    Executable entry point.

projects/

    Individual games and applications.

editor/

    Editor application.

tools/

    Development tools.

assets/

    Engine and project assets.

tests/

    Automated tests.

docs/

    Documentation.
```

## Architectural Rules

- The engine is reusable.
- Projects are replaceable.
- Gameplay belongs to projects.
- Engine functionality belongs to the engine.
- Systems belong to the engine.
- Systems provide reusable functionality.
- Runtime remains generic.
- Application owns the project boundary.
- Projects connect through the project interface.
- The engine never depends on projects.
- Editor functionality remains separate from runtime functionality.
