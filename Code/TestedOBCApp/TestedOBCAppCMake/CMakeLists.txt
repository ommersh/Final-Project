cmake_minimum_required(VERSION 3.10)
project(TestedOBCApp)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17) # Adjust the C++ standard as needed
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directories
include_directories(
                    ../CommManager
                    ../Main
                    ../Main/TestedOBCApp
                    ../TestManager
                    ../Timer
                    ../../Common
                    ../../Common/CommChannels
                    ../../Common/Logger
                    ../../Implementations
                    ../../Implementations/ANCAS
                    ../../Implementations/CATCH
                    ../../Implementations/SBO_ANCAS
                    ../../../SGP4/CPP
                    ../ExternalLibraries/eigen-3.4.0)

# Add source files
add_executable(${PROJECT_NAME}
                ../CommManager/CommManager.cpp

                ../Main/Factory.cpp
                ../Main/MainProcess.cpp
                ../Main/Main.cpp

                ../TestManager/TestManager.cpp

                ../Timer/ChronoBasedTimer.cpp

                ../../Implementations/FileReader.cpp
                ../../Implementations/ANCAS/ANCAS.cpp
                ../../Implementations/CATCH/CATCH.cpp
                ../../Implementations/CATCH/CompanionMatrixRootsFinder.cpp
                ../../Implementations/CATCH/CompanionMatrixRootsFinderEigen.cpp
                ../../Implementations/SBO_ANCAS/SboAncas.cpp
                ../../Implementations/SBO_ANCAS/SGP4SinglePointGenerator.cpp

                ../../../SGP4/CPP/SGP4.cpp
                ../../Common/SimpleDataGeneration.cpp
                ../../Common/Logger/ResultsLogger.cpp
                ../../Common/CommChannels/TestedOBCLocalSimulation.cpp)









