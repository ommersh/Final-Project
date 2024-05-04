cmake_minimum_required(VERSION 3.10)
project(TestedOBCApp)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17) # Adjust the C++ standard as needed
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Set a variable to the path of your version file
set(VERSION_HEADER "${CMAKE_SOURCE_DIR}/../Main/Version.h")
message("Version header path: ${VERSION_HEADER}")

function(parse_version_component VARIABLE COMPONENT)
    file(STRINGS ${VERSION_HEADER} VERSION_LINES REGEX "static const int ${COMPONENT} = [0-9]+;")
    if(VERSION_LINES)
        string(REGEX REPLACE ".*static const int ${COMPONENT} = ([0-9]+).*" "\\1" ${VARIABLE} "${VERSION_LINES}")
        set(${VARIABLE} ${${VARIABLE}} PARENT_SCOPE)  # Ensure variable is set in parent scope
        message("Parsed ${COMPONENT}: ${${VARIABLE}}")
    else()
        message(WARNING "No line found for ${COMPONENT} in ${VERSION_HEADER}")
    endif()
endfunction()


# Parse each version component
parse_version_component(MAJOR_VERSION "VERSION_MAJOR")
parse_version_component(MINOR_VERSION "VERSION_MINOR")
parse_version_component(PATCH_VERSION "VERSION_PATCH")

# Combine version components into a full version string
set(TESTEDOBCAPP_VERSION "${MAJOR_VERSION}_${MINOR_VERSION}_${PATCH_VERSION}")
message("Configuring build for TestedOBCApp version ${TESTEDOBCAPP_VERSION}")

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
                    ../../Common/Enums
                    ../../Common/Logger
                    ../../Common/Utils
                    ../../Implementations
                    ../../Implementations/ANCAS
                    ../../Implementations/CATCH
                    ../../Implementations/SBO_ANCAS
                    ../../../SGP4/CPP
                    ../../ExternalLibraries/INIH
                    ../../ExternalLibraries/INIH/cpp
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


                ../../Common/CommChannels/TcpCommChannel.cpp
                ../../Common/DataGeneration/SimpleDataGeneration.cpp
                ../../Common/DataGeneration/FullCatalogTestDataGeneration.cpp
                ../../../SGP4/CPP/SGP4.cpp
                ../../../SGP4/CPP/DataGenerator.cpp
                ../../../SGP4/CPP/AncasDataGenerator.cpp
                ../../../SGP4/CPP/CatchDataGenerator.cpp

                ../../ExternalLibraries/INIH/ini.c
                ../../ExternalLibraries/INIH/cpp/INIReader.cpp

                ../../Common/Logger/ResultsLogger.cpp
                )

# Add preprocessor define(s)
target_compile_definitions(${PROJECT_NAME} PRIVATE TestedOBCApp)

set_target_properties(TestedOBCApp PROPERTIES OUTPUT_NAME "TestedOBCApp_v_${TESTEDOBCAPP_VERSION}")









