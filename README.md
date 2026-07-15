# Hyro

Hyro is a hobby 3D rendering engine written in C++. The project focuses on exploring modern graphics programming, rendering architectures, and low-level graphics APIs.

The project originally started as an experiment to create a custom water simulation. During this process, I decided to build the underlying rendering technology myself instead of relying on existing high-level frameworks. This led to the development of a custom rendering architecture based on OpenGL and Vulkan.

Hyro is primarily developed as a learning project to experiment with rendering techniques and engine architecture. The long-term goal is to create a flexible 3D renderer featuring physically based rendering (PBR) and a clean abstraction layer for different graphics APIs.

## Features

Currently implemented:

- C++ based rendering architecture
- OpenGL and Vulkan graphics backends
- Shader abstraction and reflection system for OpenGL and Vulkan
- Texture and material system
- Input handling
- ImGui integration
- Basic scene and entity system
- Model loading system (work in progress)

The engine is currently under active development, with a focus on expanding the rendering pipeline and improving the overall architecture.

## Getting Started

### Requirements

- Windows
- Visual Studio 2022 or newer
- Vulkan SDK

Hyro is currently only tested on Windows.

### 1. Downloading the repository

Clone the repository using:

```bash
git clone --recursive https://github.com/BaslOr/Hyro-Engine
```

If the repository was cloned non-recursively previously, use `git submodule update --init` to clone the necessary submodules.

<ins>**2. Configuring the dependencies:**</ins>

1. Run the [Setup.py](https://github.com/BaslOr/Hyro-Engine/blob/main/scripts/SetUp.py) script found in `scripts` folder. This will download the required prerequisites for the project if they are not present yet.
2. One prerequisite is the Vulkan SDK. If it is not installed, the script will execute the `VulkanSDK.exe` file, and will prompt the user to install the SDK.
3. After all prerequisites are downloaded the pyhton script compiles any external library that uses CMake as a build system (this may take a while). The remaining projects using Premake will be generated together with the engine project files.
4. After this process you can either execute an already existing build script for Instance [GenVS2022.bat](https://github.com/BaslOr/Hyro-Engine/tree/main/scripts/GenVS2022.bat) or run Premake yourself.

If changes are made, or if you want to regenerate project files, rerun one of the `GenXXX` files found inside the `scripts` folder or rerun Premake.

## Rendering Architecture

Hyro is designed around a low-level rendering abstraction that allows the engine to interact with different graphics APIs through a unified interface.

The renderer provides abstractions for common graphics resources such as:

- Vertex buffers
- Index buffers
- Uniform buffers
- Shaders
- Textures
- Materials

API-specific code is isolated in the backend layer, where operations such as command submission and resource creation are handled.


## Current Development

Hyro is currently being expanded from a general rendering framework into a more complete physically based rendering pipeline.

Current development focuses on:
- PBR materials
- Improved scene management
- More advanced rendering features
- Better resource handling