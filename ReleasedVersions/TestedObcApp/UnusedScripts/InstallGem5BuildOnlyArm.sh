#!/bin/bash

gem5_dir="./Gem5"
cd "$gem5_dir"
echo "The current working directory is:"
pwd

#Build the Gem5 Project for Arm
echo 
echo "Starting the build of Gem5 for ARM..."
cd ./gem5
(echo && echo y) | scons build/ARM/gem5.opt -j$(nproc)
cd ..
echo "Build completed successfully."

cd ..