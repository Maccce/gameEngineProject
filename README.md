# gameEngineProject

A custom C++20 2D game engine project built from the ground up with a focus on modular architecture, open-source dependencies, and long-term maintainability.

The goal is to create a reusable game engine foundation that can support:

* 2D games
* editor tooling
* scripting
* asset pipelines
* cross-platform builds

The project is designed to run on:

* Windows 11
* Linux
* WSL2 development environments
* Docker-based CI builds

---

# Project Goals

The engine is designed around these principles:

* **Modular architecture**
  Engine systems should be independent and replaceable.

* **Separation between engine and games**
  Games should use the engine without being tightly coupled to its implementation.

* **Modern C++ practices**
  The project uses C++20 and modern build tooling.

* **Open-source friendly foundation**
  Dependencies are chosen with commercial and open-source usage in mind.

* **Reproducible builds**
  Development and CI environments should produce consistent results.

---

# Architecture Overview

The repository is organized into several major components:

```
gameEngineProject/

├── engine/        Core engine library
├── runtime/       Game runtime executable
├── editor/        Engine editor tools
├── tools/         Asset and development tools
├── projects/      Games using the engine
├── assets/        Game and engine assets
├── shaders/       GPU shaders
├── tests/         Automated tests
├── docs/          Documentation
└── cmake/         Build configuration
```

---

# Engine Architecture

The intended runtime structure:

```
Game

 |

 v

Runtime

 |

 v

Engine Core

 +----------------+
 |                |
 v                v

 ECS          Renderer

 Physics      Audio

 Input        Scripting

 Assets       Editor
```

The engine is built as a reusable library. The runtime layer provides the executable entry point.

---

# Current Status

The project is currently in the foundation stage.

Completed:

* Repository architecture
* CMake build system
* C++20 configuration
* Engine static library
* Runtime executable
* Initial application lifecycle

Current test:

```
Engine initialized
Engine shutdown
```

Planned systems:

* SDL3 window management
* Input system
* Game loop
* bgfx rendering backend
* EnTT ECS integration
* Asset management
* Dear ImGui editor
* Python scripting
* Hot reload system
* Physics integration

---

# Build Requirements

## Linux / WSL2

Required:

* CMake
* Ninja
* C++20 compiler
* Git

Example Ubuntu installation:

```bash
sudo apt update

sudo apt install \
    cmake \
    ninja-build \
    build-essential \
    git
```

---

# Building

Configure:

```bash
cmake -S . -B build -G Ninja
```

Build:

```bash
cmake --build build
```

Run:

```bash
./build/bin/gameRuntime
```

Expected output:

```
Engine initialized
Engine shutdown
```

---

# Development Dependencies

The project uses external libraries managed through CMake and vcpkg.

Planned dependencies include:

* SDL3 - windowing and input
* bgfx - rendering abstraction
* EnTT - entity component system
* pybind11 - Python integration
* Box2D - 2D physics
* Dear ImGui - editor interface

---

# Directory Guidelines

## engine/

Contains reusable engine code.

Examples:

* rendering
* ECS
* physics
* audio
* scripting
* resource management

The engine should not depend on any specific game.

---

## runtime/

Contains the executable layer.

Responsible for:

* starting the engine
* loading a project
* running the game loop

---

## projects/

Contains games or applications built using the engine.

Example:

```
projects/

└── ExampleGame/

    ├── src/
    ├── scripts/
    └── assets/
```

---

# Build Files

Generated files should not be committed.

Ignored:

```
build/
CMakeCache.txt
CMakeFiles/
*.exe
*.dll
*.o
```

Source files and build configuration belong in Git.

---

# License

This project license has not yet been finalized.

Third-party dependencies retain their respective licenses.

---

# Roadmap

## Foundation

* [x] Repository structure
* [x] CMake setup
* [x] Engine library
* [x] Runtime executable

## Core Engine

* [ ] Main game loop
* [ ] Time system
* [ ] Logging
* [ ] Event system
* [ ] Input handling

## Rendering

* [ ] SDL3 integration
* [ ] bgfx integration
* [ ] Sprite rendering
* [ ] Camera system

## Gameplay

* [ ] ECS integration
* [ ] Scene system
* [ ] Physics
* [ ] Animation

## Tools

* [ ] Editor
* [ ] Asset pipeline
* [ ] Hot reload
* [ ] Packaging tools
