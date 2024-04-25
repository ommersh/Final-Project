#!/bin/bash

echo 
echo "Installing Requirements"
sudo apt install git
sudo apt install build-essential
sudo apt install scons
sudo apt install python3-dev
(echo y) | sudo apt install libboost-all-dev
sudo apt-get install automake

echo "Installing Requirements Completed"


# Define the path for the Gem5 directory
gem5_dir="./Gem5"
echo 
# Check if the directory exists
if [ -d "$gem5_dir" ]; then
    echo "The Gem5 directory exists. Emptying..."
    # Remove all files and directories, including hidden ones
    rm -rf "$gem5_dir"/*
    rm -rf "$gem5_dir"/.??*
else
    echo "The Gem5 directory does not exist. Creating..."
    # Create the directory
    mkdir -p "$gem5_dir"
fi

echo "Operation completed. Foledr created"

#Go to the Gem5 folder
cd "$gem5_dir"
echo "The current working directory is:"
pwd
#We have our folder, try to clone the gem5 project
echo 
echo "Cloning the Gem5 project, it might take some time"
git clone https://github.com/gem5/gem5

# Check if the git clone command was successful
if [ $? -eq 0 ]; then
    echo "Repository successfully cloned"
else
    echo "Failed to clone the repository. Please check your network connection and the repository URL."
    exit 1
fi

echo "Repository successfully cloned"

#Build the Gem5 Project for Arm
echo 
echo "Starting the build of Gem5 for X86..."
cd ./gem5
(echo && echo y) | scons build/X86/gem5.opt -j$(nproc)
cd ..
echo "Build completed successfully."

cd ..