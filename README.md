Vulkan & Graphics Learning project
===

I'm trying to learn many things within one project: 
- C++ 
- Vulkan
- How graphics are working
- How a game engine works
- Create some experiments while improving / rewriting parts of the engine

If you would like to spare some time with Vulkan or you feel you can reuse something from the engine feel free, currently I'm not intending to close the source at all, maybe just a future game I would like to work on will be closed source, but never the engine. 
**I have no intent to create a widely used engine, this is just a disclaimer for you, so everything is as is.** There are a lot of publicly available engines if you would like to touch on game development, I would advise you to take a look at those. 

If you are still here that means you want to try it out maybe. I'll try not to forget to update the below instructions in the future. 

# Prerequisites
Using C++ so you'll need to have a working compiler and C++ environment being set up, I won't detail it, it's your decision how to do it. 
This project uses CMake, please install it before proceeding.  
In order to use Vulkan you'll need to install the [SDK](https://vulkan.lunarg.com/sdk/home) first for your OS. 

## Quick Start

### Automated Build (Recommended)

The easiest way to build and run the project:

```sh
./build.sh    # Builds the project, compiles shaders, and copies assets
./run.sh      # Runs the application
```

### Manual Build

If you prefer to build manually or the scripts don't work on your system:

1. Create a build directory:
```sh
mkdir build
cd build
```

2. Configure and build with CMake:
```sh
cmake ..
cmake --build . -j$(nproc)  # Linux
# or
cmake --build . -j$(sysctl -n hw.ncpu)  # macOS
```

3. Compile shaders (requires `glslc` from Vulkan SDK):
```sh
mkdir shaders
glslc ../shaders/triangle.vert -o shaders/vert.spv
glslc ../shaders/triangle.frag -o shaders/frag.spv
```

4. Copy assets to build directory:
```sh
cp -R ../models .
cp -R ../textures .
```

5. Run the application:
```sh
./DarkestPlanet
```

> **NOTE** 
> `glslc` comes with the Vulkan SDK. Ensure the SDK is installed and `glslc` is in your PATH. Visit [Vulkan SDK](https://vulkan.lunarg.com/sdk/home) for installation instructions.

## Current Status

The engine currently implements:
- ✅ Basic Vulkan rendering pipeline
- ✅ 3D model loading (OBJ files via tinyobjloader)
- ✅ Texture loading and sampling
- ✅ Depth testing
- ✅ Window management with GLFW
- ✅ Driver abstraction (Vulkan implementation)

**Current Limitations:**
- Single model rendering (hardcoded model path)
- Manual shader compilation required
- No scene management or entity system
- No camera system (hardcoded view/projection)

See [ASSESSMENT.md](ASSESSMENT.md) for a detailed project assessment and [VOXEL_RENDERING.md](VOXEL_RENDERING.md) for voxel rendering implementation ideas.



