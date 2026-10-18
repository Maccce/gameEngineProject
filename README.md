# gameEngineProject

A custom C++23 2D game engine focused on modular architecture, reusable engine systems, modern C++, and reproducible development environments.

The engine is intended to support games, editor tooling, asset pipelines, scripting, and cross-platform builds.

Development uses Docker and Visual Studio Code Dev Containers.

**Development environments**
- Linux
- Windows 11 with WSL2
- Docker-based development and CI

Architecture details: [`docs/architecture.md`](docs/architecture.md)

## Project Goals

- **Modular architecture:** Keep engine functionality separated into reusable systems.
- **Engine/project separation:** Projects use the engine without embedding engine implementation.
- **Modern C++:** Use C++23, CMake, Ninja, and vcpkg.
- **Reproducible builds:** Use a consistent containerized development environment.

## Repository Structure

```text
gameEngineProject/
├── engine/          Reusable engine functionality and systems
├── application/     Application lifecycle and project management
├── runtime/         Generic executable entry point
├── projects/        Games and applications using the engine
├── editor/          Editor application
├── tools/           Development and asset pipeline tools
├── assets/          Engine and project assets
├── shaders/         GPU shader programs
├── tests/           Automated tests
├── scripts/         Development scripts
├── cmake/           CMake modules and configuration
├── docker/          Docker configuration
├── .devcontainer/   VS Code Dev Container configuration
├── docs/            Documentation
└── .github/         GitHub Actions workflows
```

## Architecture Overview

The project separates responsibilities across four main components:

- **Runtime:** Starts the application and returns its exit code.
- **Application:** Manages application lifecycle, project discovery, selection, loading, and project lifecycle coordination.
- **Engine:** Provides reusable systems and manages their lifecycle.
- **Projects**: Implement the C-compatible project API and provide project-specific behaviour.

Projects are discovered through their `project.json` metadata. The application loads the selected project's shared library through a generic module entry point. The application does not depend on concrete project classes.

See [`docs/architecture.md`](docs/architecture.md) for ownership rules and dependency relationships.

## Editor

The editor is intended to be a separate application built using the engine.

Planned functionality:
- Scene editing
- Entity inspection
- Asset management
- Debug tooling

The editor structure may include:

```text
editor/
├── gizmos/
├── panels/
└── windows/
```

## Tools

Development tools support workflows such as:

- Asset processing
- Project generation
- Shader compilation
- Packaging

Planned structure:

```text
tools/
├── asset_cooker/
├── project_generator/
└── shader_compiler/
```

## Assets

Source assets and processed runtime data are kept separate:

```text
assets/
├── raw/
└── cooked/
```

## Development Environment

The development environment uses Docker through Visual Studio Code Dev Containers.

The container provides:
- Ubuntu 24.04
- GCC
- CMake
- Ninja
- Python
- vcpkg
- Required build dependencies

The same environment is intended for local development and CI builds. A local C++ toolchain, CMake, Ninja, or vcpkg installation is not required on the host.

### Host Requirements

- Docker
- Docker Compose
- Visual Studio Code
- Dev Containers extension
- Git
- GitHub SSH key and configured SSH agent

### Open the Development Container

1. Start the GitHub SSH agent on the host.
2. Open the repository in VS Code from the same environment.
3. Open the command palette with `Ctrl + Shift + P`.
4. Select **Dev Containers: Rebuild and Reopen in Container**.

## GitHub SSH Setup

The container uses SSH agent forwarding. The private key remains on the host; the container accesses the running agent instead of receiving a copy of the key.

1. On the host, create a key if needed:

    ```bash
    ssh-keygen -t ed25519 -C "your-email@example.com"
    ```

2. Start the SSH agent:

    ```bash
    eval "$(ssh-agent -s)"
    ```

3. Add your key:

    ```bash
    ssh-add ~/.ssh/id_ed25519
    ```

4. Remove old stale socket:

    ```bash
    rm -f ~/.ssh/agent.sock
    ```

5. creates a symbolic link (symlink) from a fixed path to your current SSH agent socket:

    ```bash
    ln -s $SSH_AUTH_SOCK ~/.ssh/agent.sock
    ```

6. Verify the agent:

    ```bash
    ssh-add -l
    ```

7. Add the public key to **GitHub → Settings → SSH and GPG keys** if you have not already done so.

8. Test authentication on the host and inside the container:

    ```bash
    ssh -T git@github.com
    ```

    Expected:
    ```bash
    Hi <username>! You've successfully authenticated, but GitHub does not provide shell access.
    ```

## Verify the Development Environment

Run these commands inside the Dev Container:

```bash
g++ --version
```

Check CMake:

```bash
cmake --version
```

Check Ninja:

```bash
ninja --version
```

Check vcpkg:

```bash
$VCPKG_ROOT/vcpkg version
```

Check GitHub authentication:

```bash
ssh -T git@github.com
```

## Building and Running

All build commands should be executed inside the container from the repository root.

### Configure

```bash
cmake -S . -B build -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake"
```

### Build

```bash
cmake --build build
```

### Run

```bash
./build/bin/gameRuntime
```

The application discovers available projects, displays their metadata, and prompts for a selection. Choose a listed project to load and run it.

Example output (frame timing and log timestamps vary):

```text
[info] Engine initialized
Available Projects

1. Example Game
   Description: An example game project.
   Version: 0.1.0
   Path: "projects/ExampleGame"

Select project: 1

Selected: Example Game
ExampleGame initialized
ExampleGame update
Frame delta: 0.100162
...
ExampleGame shutdown
[info] Engine shutdown
```

## Dependencies

Dependencies are managed through vcpkg.

Current and planned dependencies include:

- SDL3 — windowing and input
- bgfx — rendering abstraction
- EnTT — entity component system
- pybind11 — Python integration
- Box2D — physics
- Dear ImGui — editor interface

## Current Status

Implemented:
- Initial repository and development-container setup
- CMake and C++23 configuration
- vcpkg integration
- Engine static library and runtime executable
- Engine initialization and shutdown
- Time system
- Logging foundation
- Application framework and C-compatible project API
- Project metadata and registry
- Project discovery and selection
- Dynamic project module loading
- Project creation and destruction through the C-compatible API
- Example project integration
- Application, engine, and project lifecycle coordination
- Project callback error reporting and propagation
- Project-selection input validation

Planned:
- Runtime project switching
- Input and event systems
- Rendering and window integration
- ECS and scene systems
- Physics and animation
- Editor functionality
- Asset processing and packaging tools
- Automated tests

## Roadmap

### Application
- [x] Project discovery
- [x] Project selection
- [x] Dynamic project module loading
- [x] Project lifecycle coordination
- [x] Project-selection input validation
- [x] Project callback error propagation
- [ ] Expanded error-path testing
- [ ] Runtime project switching

### Core Engine
- [x] Time system
- [x] Initial logging support
- [ ] Logging improvements
- [ ] Event system
- [ ] Input handling

### Rendering
- [ ] SDL3 integration
- [ ] bgfx integration
- [ ] Sprite rendering
- [ ] Camera system

### Gameplay Systems
- [ ] ECS integration
- [ ] Scene system
- [ ] Physics
- [ ] Animation

### Editor
- [ ] Scene editor
- [ ] Asset browser
- [ ] Entity inspector
- [ ] Debug visualization

### Tools
- [ ] Asset pipeline improvements
- [ ] Hot reload workflow
- [ ] Packaging tools

## License

The project license has not yet been finalized.

Third-party dependencies retain their respective licenses.
