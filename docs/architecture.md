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
    Discovers and loads projects.

projects/

    Contains games and applications using the engine.

engine/

    Provides reusable engine and systems functionality.

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
- project-specific code,
- project selection logic,
- project loading logic.

Project selection and loading are handled by the application layer.

### Application

The application layer provides the framework for running an engine-based application.

It manages the relationship between runtime, projects, and engine startup.

Responsibilities:

- Manage application lifetime.
- Coordinate engine startup and shutdown.
- Own the project interface.
- Discover available projects.
- Select a project.
- Load and run a selected project.
- Provide the application environment required by projects.

The application layer does not contain:

- gameplay rules,
- project-specific behaviour,
- engine system implementations.

#### Project Registry

The project registry is responsible for discovering available projects.

Physical location:

```text
application/
    ProjectRegistry.hpp
    ProjectRegistry.cpp
    ProjectInfo.hpp
```

Responsibilities:

- Search a provided projects directory.
- Read project metadata.
- Create project information entries.
- Provide a list of available projects.

The project registry does not:

- load project modules,
- create project instances,
- contain project-specific knowledge.

Example:

```text
Available Projects

1. Example Game
   Description: An example game project.
   Version: 0.1.0
   Path: projects/ExampleGame

2. Another Game
   Description: An other game project.
   Version: .0.1.0
   Path: projects/AnotherGame
```

The application decides which project is selected.

#### Project Loader

The project loader is responsible for loading a selected project into the application.

Physical location:

```text
application/

    ProjectLoader.hpp
    ProjectLoader.cpp
```

Responsibilities:

- Receive selected project information.
- Locate the project module.
- Load the project module.
- Create the project instance.
- Provide the project instance to the application.

Project loading is an application responsibility.

The engine does not know how projects are discovered or loaded.

#### Project Module

Projects provide a module entry point used to create a project instance.

Relationship:

```text
Application
    |
    v
Project Loader
    |
    v
Project Module
    |
    v
Project Implementation
    |
    v
Project Interface
```

The project module is responsible for creating the concrete project implementation.

The application does not depend on concrete project classes.

The project module belongs to the project/application boundary and is not part of the engine.

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
Application
    |
    v
Engine
    |
    v
Systems
```

#### Engine Modules

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

#### Systems

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
- provide project modules.
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

The architecture uses diagrams with a consistent notation.

Diagram rules:

- Arrows (`->`) represent dependency direction.
- The source depends on the destination.
- Implementation relationships are shown separately using `implements`.
- Creation/loading flow is described as a sequence, not as dependency arrows.

### Dependency Relationship

```text
Runtime
    |
    v
Application
    |
    +----------------+
    |                |
    v                v
Project Loader     Engine
    |
    v
Project Module
    |
    v
Project Implementation


Project Implementation
        |
        | implements
        v

Project Interface
```

The dependency relationships are:

```text
Runtime
    -> Application

Application
    -> Project Loader
    -> Engine
    -> Project Interface

Project Loader
    -> Project Module

Project Module
    -> Project Implementation

Project Implementation
    -> Engine
    -> Project Interface
```

### Project Loading Flow

The loading sequence is:

```text
Runtime starts Application

Application
    |
    v

Project Registry
    |
    v

Available Projects

Application selects a project

Application
    |
    v

Project Loader
    |
    v

Project Module
    |
    v

Project Implementation
```

The loading flow describes execution order.

It does not represent ownership or dependency direction.

### Project Usage Flow

```text
Project Implementation
        |
        | uses
        v

Engine
        |
        v

Systems
```

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

### Architectural Rules

Projects use engine functionality.

The engine does not depend on projects.

The important architectural rules are:

- The engine is reusable.
- Projects are replaceable.
- Gameplay belongs to projects.
- Engine functionality belongs to the engine.
- Systems belong to the engine.
- Systems provide reusable functionality.
- Runtime remains generic.
- Runtime starts the application.
- Application owns the project boundary.
- Application defines the project interface.
- Projects implement the project interface.
- Application discovers and loads projects.
- Project modules create project implementations.
- Projects use engine functionality.
- Engine never depends on projects.
- Editor functionality remains separate from runtime functionality.
