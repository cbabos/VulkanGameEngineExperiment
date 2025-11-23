#!/bin/bash

# Build script for Darkest Planet Engine
# This script configures CMake, builds the project, compiles shaders, and copies assets

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR"
BUILD_DIR="$PROJECT_ROOT/build"

echo "=== Darkest Planet Engine Build Script ==="
echo ""

# Check for Vulkan SDK
if ! command -v glslc &> /dev/null; then
    echo "ERROR: glslc not found. Please install Vulkan SDK and ensure it's in your PATH."
    echo "Visit: https://vulkan.lunarg.com/sdk/home"
    exit 1
fi

# Create build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir -p "$BUILD_DIR"
fi

# Configure CMake
echo "Configuring CMake..."
cd "$BUILD_DIR"
cmake "$PROJECT_ROOT"

# Build the project
echo ""
echo "Building project..."
# Detect number of CPU cores (works on both macOS and Linux)
if command -v nproc &> /dev/null; then
    CORES=$(nproc)
elif command -v sysctl &> /dev/null; then
    CORES=$(sysctl -n hw.ncpu)
else
    CORES=4
fi
cmake --build . -j$CORES

# Create shaders directory in build
SHADER_DIR="$BUILD_DIR/shaders"
if [ ! -d "$SHADER_DIR" ]; then
    echo "Creating shaders directory..."
    mkdir -p "$SHADER_DIR"
fi

# Compile shaders
echo ""
echo "Compiling shaders..."
glslc "$PROJECT_ROOT/shaders/triangle.vert" -o "$SHADER_DIR/vert.spv"
glslc "$PROJECT_ROOT/shaders/triangle.frag" -o "$SHADER_DIR/frag.spv"

if [ $? -eq 0 ]; then
    echo "✓ Shaders compiled successfully"
else
    echo "✗ Shader compilation failed"
    exit 1
fi

# Copy assets (models and textures)
echo ""
echo "Copying assets..."
if [ -d "$PROJECT_ROOT/models" ]; then
    cp -R "$PROJECT_ROOT/models" "$BUILD_DIR/"
    echo "✓ Models copied"
fi

if [ -d "$PROJECT_ROOT/textures" ]; then
    cp -R "$PROJECT_ROOT/textures" "$BUILD_DIR/"
    echo "✓ Textures copied"
fi

echo ""
echo "=== Build Complete ==="
echo "Executable: $BUILD_DIR/DarkestPlanet"
echo ""
echo "To run the application:"
echo "  cd $BUILD_DIR"
echo "  ./DarkestPlanet"
echo ""
echo "Or use: ./run.sh"

