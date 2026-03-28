#!/bin/bash

set -e

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Run CMake to configure the project
cmake ..

# Build the project
make

# copy the built module to the parent directory for easy import in Python
cp bd1_sim.* ..

echo "Build complete. The bd1_sim module is now available in the parent directory."
