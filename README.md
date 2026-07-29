# gameEngineProject

A custom C++20 2D game engine built from the ground up with a focus on modular architecture, modern tooling, and long-term maintainability.

The goal is to create a reusable engine foundation capable of supporting:

* 2D games
* Editor tooling
* Asset pipelines
* Scripting
* Cross-platform development

The project is designed to support development on:

* Windows 11
* Linux
* WSL2
* Docker Dev Containers
* Docker-based CI builds

---

# Project Goals

The engine is designed around several core principles.

### Modular architecture

Engine systems should be independent, reusable, and replaceable.

### Separation of engine and game code

Games should depend on the engine without coupling to its implementation.

### Modern C++

The project uses C++20 together with modern CMake practices.

### Open-source ecosystem

Dependencies are managed through **vcpkg** and chosen with commercial and open-source licensing in mind.

### Reproducible development

Every developer works in the same Docker-based environment to eliminate "works on my machine" problems.

---

# Repository Structure

```text
gameEngineProject/

├── assets/          Engine and game assets
├── cmake/           CMake helper files
├── docker/          Docker development environment
├── docs/            Documentation
├── editor/          Editor application
├── engine/          Engine library
├── projects/        Games using the engine
├── runtime/         Runtime executable
├── shaders/         GPU shaders
├── tests/           Automated tests
├── CMakeLists.txt
└── vcpkg.json
```

---

# Architecture

```text
Game

 │

 ▼

Runtime

 │

 ▼

Engine Core

 ├── ECS
 ├── Rendering
 ├── Input
 ├── Audio
 ├── Physics
 ├── Assets
 ├── Scripting
 └── Editor
```

The engine is built as a reusable static library while the runtime provides the executable entry point.

---

# Current Status

Completed:

* Repository structure
* CMake build system
* Docker development environment
* vcpkg dependency management
* Engine static library
* Runtime executable
* Logging
* Basic engine lifecycle

Current output:

```text
Engine initialized
Frame delta: ...
Engine shutdown
```

Planned systems:

* SDL3 window management
* bgfx renderer
* EnTT ECS
* Input system
* Asset management
* Dear ImGui editor
* Python scripting
* Hot reload
* Physics integration

---

# Development Environment

Development is performed inside a Docker Dev Container.

The container includes:

* GCC
* CMake
* Ninja
* Git
* Python
* vcpkg
* All required build tools

This provides a reproducible environment across Windows, Linux, and WSL2.

---

# Getting Started

## Prerequisites

Install:

* Docker Desktop (Windows) or Docker Engine (Linux)
* Visual Studio Code
* Dev Containers extension

Clone the repository:

```bash
git clone <repository-url>
cd gameEngineProject
```

Open the repository in VS Code and select:

**Dev Containers: Reopen in Container**

The development container will be built automatically.

---

# Building

Inside the Dev Container:

Configure:

```bash
cmake -S . -B build -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

Build:

```bash
cmake --build build
```

Run:

```bash
./build/bin/gameRuntime
```

---

# Dependencies

Dependencies are managed through **vcpkg** in manifest mode.

Current and planned libraries include:

* SDL3
* bgfx
* EnTT
* spdlog
* Dear ImGui
* pybind11
* Box2D

No manual dependency installation is required inside the development container.

---

# Project Layout

## engine/

Reusable engine functionality.

Examples:

* Rendering
* ECS
* Audio
* Physics
* Resource management
* Scripting

---

## runtime/

Application entry point.

Responsible for:

* Engine startup
* Project loading
* Main loop

---

## editor/

Future editor application.

Planned features:

* Scene editing
* Asset browser
* Inspector
* Debug tools

---

## projects/

Games built using the engine.

Example:

```text
projects/

└── ExampleGame/
    ├── assets/
    ├── scripts/
    └── src/
```

---

# Generated Files

Generated files should never be committed.

Examples:

```text
build/
CMakeCache.txt
CMakeFiles/
```

---

# Roadmap

## Foundation

* [x] Repository structure
* [x] CMake build system
* [x] Docker development environment
* [x] vcpkg integration
* [x] Engine library
* [x] Runtime executable

## Core Engine

* [ ] Main loop
* [ ] Time system
* [ ] Event system
* [ ] Input handling

## Rendering

* [ ] SDL3
* [ ] bgfx
* [ ] Sprite renderer
* [ ] Camera system

## Gameplay

* [ ] ECS
* [ ] Scene system
* [ ] Physics
* [ ] Animation

## Tools

* [ ] Editor
* [ ] Asset pipeline
* [ ] Hot reload
* [ ] Packaging

---

# License

License selection is still pending.

Third-party libraries remain under their respective licenses.
