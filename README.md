# gameEngineProject

A custom C++20 2D game engine built from the ground up with a focus on modular architecture, reusable engine systems, modern C++ practices, and reproducible development environments.

The goal is to create a reusable engine foundation capable of supporting:

- 2D games
- editor tooling
- asset pipelines
- scripting
- cross-platform builds

The project uses a Docker-based development environment to provide consistent builds across machines.

Supported development environments:

- Windows 11 + WSL2
- Linux
- Docker-based CI environments

Detailed architecture documentation:

```
docs/architecture.md
```

## Project Goals

The engine is designed around these principles:

### Modular architecture

Engine systems are separated into independent modules that can evolve without tightly coupling the entire codebase.

### Separation between engine and projects

Projects use the engine as a reusable foundation rather than containing engine-specific implementation.

### Modern C++ practices

The project uses:

- C++20
- CMake
- Ninja
- vcpkg

### Reproducible builds

The development environment is containerized so developers and CI systems use the same dependencies.

## Repository Structure

```text
gameEngineProject/

├── engine/ Core engine functionality
├── application/ Application framework and project integration
├── runtime/ Executable entry point
├── projects/ Games and applications using the engine
├── editor/ Editor application
├── tools/ Development and asset pipeline tools
├── assets/ Engine and project assets
├── shaders/ GPU shader programs
├── tests/ Automated tests
├── scripts/ Development scripts
├── cmake/ CMake modules and configuration
├── docker/ Docker configuration
├── .devcontainer/ VS Code Dev Container configuration
├── docs/ Documentation
└── .github/ GitHub Actions workflows
```

For architecture decisions and ownership rules, see:

```text
docs/architecture.md
```

## Editor

The editor contains engine development tools.

Current structure:

```text
editor/
├── gizmos/
├── panels/
└── windows/
```

Planned functionality:

- Scene editing
- Entity inspection
- Asset management
- Debug tooling

## Tools

Development tools are located in:

```text
tools/
├── asset_cooker/
├── project_generator/
└── shader_compiler/
```

These tools support:

- Asset processing
- Project creation
- Shader workflows

## Assets

Assets are separated into source and processed data:

```text
assets/
├── raw/
└── cooked/
```

The goal is to keep source assets separate from runtime-ready assets.

## Development Environment

The project uses a Docker-based development environment through Visual Studio Code Dev Containers.

The container provides:

- Ubuntu 24.04
- GCC compiler toolchain
- CMake
- Ninja
- Python
- vcpkg
- Required build dependencies

The same container image is used for:

- Local development through VS Code Dev Containers
- Automated CI builds

No local C++ compiler, CMake, Ninja, or vcpkg installation is required.

## Requirements

Install on the host machine:

- Docker
- Docker Compose
- Visual Studio Code
- Dev Containers extension
- Git
- GitHub SSH key
- SSH agent configured on the host machine

## Opening the Development Container

All build commands should be executed inside the container terminal.

1. Start the GitHub SSH agent inside a terminal outside VS Code.

2. Open the repository in VS Code from the same terminal.

3. Open the command palette:

```text
Ctrl + Shift + P
```

4. Select:

```text
Dev Containers: Rebuild and Reopen in Container
```

## GitHub SSH Setup

The development container uses SSH agent forwarding.

The SSH private key remains on the host machine and is never copied into the container.

The container only receives access to the running SSH agent socket.

### SSH agent

On the host machine:

1. Create an SSH key if needed:

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

6. Verify:

    ```bash
    ssh-add -l
    ```

7. Copy your public key (Only needed to do this first time):

    ```
    GitHub → Settings → SSH and GPG keys
    ```

8. Test:

    ```bash
    ssh -T git@github.com
    ```

    Expected:
    ```bash
    Hi <username>! You've successfully authenticated, but GitHub does not provide shell access.
    ```

### Using Git inside the container

After opening the Dev Container:

Test authentication:

```bash
ssh -T git@github.com
```

Then use Git normally.

## Verify Development Environment

Inside the Dev Container terminal:

Check compiler:

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

## Building

All commands are run inside the container.

### Configure

```bash
cmake -S . -B build -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

### Build

```bash
cmake --build build
```

### Run

```bash
./build/bin/gameRuntime
```

Expected output:

```text
[info] Engine initialized

Frame delta: ...

[info] Engine shutdown
```

## Dependencies

Dependencies are managed through vcpkg.

Current and planned dependencies include:

- SDL3 - windowing and input
- bgfx - rendering abstraction
- EnTT - entity component system
- pybind11 - Python integration
- Box2D - physics
- Dear ImGui - editor interface

## Current Status

Completed:

- Initial repository structure
- Docker development environment
- VS Code Dev Container setup
- CMake build system
- C++20 configuration
- Engine static library
- Runtime executable
- vcpkg integration
- Modular engine layout
- Editor structure
- Tool structure
- Test structure

## Roadmap

### Application

- [ ] Application lifecycle
- [ ] Project interface
- [ ] Project loading
- [ ] Application loop

### Core Engine

- [ ] Time system
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

- [ ] Improved asset pipeline
- [ ] Hot reload workflow
- [ ] Packaging tools

## License

This project license has not yet been finalized.

Third-party dependencies retain their respective licenses.
