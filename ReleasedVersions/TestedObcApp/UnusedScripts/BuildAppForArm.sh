#!/bin/bash

# Install CMake if it's not already installed
if ! command -v cmake &> /dev/null
then
    echo "CMake could not be found. Installing CMake..."
    sudo apt-get update
    sudo apt-get install cmake -y
fi

# Check if the ARM cross-compiler is installed and install if it's not
if ! command -v arm-linux-gnueabi-gcc &> /dev/null; then
    echo "ARM cross-compiler not found, installing..."
    sudo apt-get install gcc-arm-linux-gnueabi g++-arm-linux-gnueabi -y
else
    echo "ARM cross-compiler is already installed."
fi

# Define the directory containing CMakeLists.txt
cmakeListDir="./../../Code/TestedOBCApp/TestedOBCAppCMake"
build_dir="./build"


# Remove the existing build directory to ensure a clean state
echo "Removing existing build directory..."
rm -rf "$build_dir"
# Create a build directory
mkdir -p "$build_dir"

# Configure the project with maximum optimizations
echo "Configuring and building the project..."
cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=arm-linux-gnueabi-gcc \
  -DCMAKE_CXX_COMPILER=arm-linux-gnueabi-g++ \
  -DCMAKE_C_FLAGS="-static -O3" \
  -DCMAKE_CXX_FLAGS="-static -O3" \
  -DSTATIC_LINKING=ON \
  -S "$cmakeListDir" \
  -B "$build_dir"

# Build the project
echo "Building the project..."
cmake --build "$build_dir"

# Check if the build was successful
if [ $? -eq 0 ]; then
    echo "Build completed successfully."
else
    echo "Build failed. Check the output for errors."
    exit 1
fi

#Copy the binary file
echo "Copying and renaming output file..."
cd "$build_dir"
file_to_copy=$(find . -maxdepth 1 -name "TestedOBCApp*" | head -n 1)
if [ -n "$file_to_copy" ]; then
    cp "$file_to_copy" "../TestedOBCAppArm"
    echo "Output file copied and renamed to 'TestedOBCAppArm'"
else
    echo "No output file was found."
fi
