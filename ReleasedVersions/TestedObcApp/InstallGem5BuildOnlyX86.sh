#!/bin/bash

gem5_dir="./Gem5"
cd "$gem5_dir"
echo "The current working directory is:"
pwd

#Build the Gem5 Project for X86
echo 
echo "Starting the build of Gem5 for X86..."
cd ./gem5
(echo && echo y) | scons build/X86/gem5.opt -j$(nproc)
cd ..
echo "Build completed successfully."

cd ..