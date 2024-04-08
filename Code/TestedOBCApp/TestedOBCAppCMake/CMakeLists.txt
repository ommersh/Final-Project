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
                    ../LocalSimulation
                    ../../Common
                    ../../Common/CommChannels
                    ../../Common/DataGeneration
                    ../../Common/Logger
                    ../../Implementations
                    ../../Implementations/ANCAS
                    ../../Implementations/CATCH
                    ../../Implementations/SBO_ANCAS
                    ../../../SGP4/CPP
                    ../../External\ Libraries/INIH
                    ../../External\ Libraries/INIH/cpp
                    ../ExternalLibraries/eigen-3.4.0)

# Add source files
add_executable(${PROJECT_NAME}
                ../CommManager/CommManager.cpp

                ../Main/Factory.cpp
                ../Main/MainProcess.cpp
                ../Main/Main.cpp
                ../Main/AppConfigurationManager.cpp

                ../TestManager/TestManager.cpp

                ../Timer/ChronoBasedTimer.cpp

                ../LocalSimulation/TestedOBCLocalSimulation.cpp

                ../../Implementations/FileReader.cpp
                ../../Implementations/ANCAS/ANCAS.cpp
                ../../Implementations/CATCH/CATCH.cpp
                ../../Implementations/CATCH/CompanionMatrixRootsFinder.cpp
                ../../Implementations/CATCH/CompanionMatrixRootsFinderEigen.cpp
                ../../Implementations/SBO_ANCAS/SboAncas.cpp
                ../../Implementations/SBO_ANCAS/SboAncasEquallySpacedPoints.cpp
                ../../Implementations/SBO_ANCAS/SGP4SinglePointGenerator.cpp

                ../../Common/DataGeneration/SimpleDataGeneration.cpp
                ../../Common/DataGeneration/FullCatalogTestDataGeneration.cpp
                ../../../SGP4/CPP/SGP4.cpp
                ../../../SGP4/CPP/DataGenerator.cpp
                ../../../SGP4/CPP/AncasDataGenerator.cpp
                ../../../SGP4/CPP/CatchDataGenerator.cpp

                ../../External\ Libraries/INIH/ini.c
                ../../External\ Libraries/INIH/cpp/INIReader.cpp

                ../../Common/Logger/ResultsLogger.cpp
                )









