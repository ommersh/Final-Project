#!/bin/bash
set -e

# Define build directory
BUILD_DIR_LINUX="LinuxRelease"

# Clean and build for Linux
if [ -d "$BUILD_DIR_LINUX" ]; then
    rm -rf "$BUILD_DIR_LINUX"
fi

mkdir "$BUILD_DIR_LINUX"
pushd "$BUILD_DIR_LINUX"

# Configure and build for Linux
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

popd
