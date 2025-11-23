#!/bin/bash

# Run script for Darkest Planet Engine
# This script runs the built application from the build directory

set -e  # Exit on error

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/build"
EXECUTABLE="$BUILD_DIR/DarkestPlanet"

# Check if executable exists
if [ ! -f "$EXECUTABLE" ]; then
    echo "ERROR: Executable not found: $EXECUTABLE"
    echo "Please run ./build.sh first to build the project."
    exit 1
fi

# Check if shaders exist
if [ ! -f "$BUILD_DIR/shaders/vert.spv" ] || [ ! -f "$BUILD_DIR/shaders/frag.spv" ]; then
    echo "ERROR: Shaders not found in build directory."
    echo "Please run ./build.sh first to compile shaders."
    exit 1
fi

# Check if assets exist
if [ ! -d "$BUILD_DIR/models" ] || [ ! -d "$BUILD_DIR/textures" ]; then
    echo "WARNING: Models or textures directory not found in build directory."
    echo "The application may fail to load assets."
fi

# Run the application from build directory (required for asset paths)
echo "Running Darkest Planet Engine..."
echo ""
cd "$BUILD_DIR"
./DarkestPlanet

