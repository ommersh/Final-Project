# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).


## [Released]
### version 1.0.1
#### Changed 17/03/2024
- Added SBO ANCAS to the algorithms types enum
- Added everything SBO-ANCAS required to run to the TestRecipe structure
- Added SBO-ANCAS to the algorithm names in the ResultsLogger
- Added SGP4SinglePointGenerator implementing the interface
- Implemented SGP4SinglePointGenerator using the SGP4 implementation
- Added SGP4SinglePointGenerator and SboAncas to the factory
- Added SBO-ANCAS initialization and testing to the test manager


#### Changed 16/03/2024
- Added results logger
- Added number of run(repeatedly calling the algorithm on the same input)
- Added Test Name and Test ID to the tets recipe and results
- Added MinRunTime and AvgRunTime to the test results
- Added Eigen Library again to the External libraries folder

### version 1.0.0
#### Changed 07/03/2024
- Reorganized the folders and files
- Renamed some files and structures

#### Added 01/03/2024
- Changed the PointData struct to include the time point and changed the algorithms implementation and interface accordingly, updated the FileReader class accordingly
- Added CommManager class to receive incoming data using a ICommChannel and parse it + sending the results back
- Implemented the MainProcces logic
- Changed types for the WinTime from double to long int
- Added ICommChannel interface for a communication channel
- Implemented a varation of ICommChannel for local testing - LocalFileCommChannelFacade: reading the data from a file and simulating a communication channel

#### Added 27/02/2024
- Changed the arrays in CATCH to static with max size - for better performance
- Added init function to catch and RootsFindAlg
- Added the number of points to the TCA struct(results struct) for SBO-ANCAS future implementation
- Changed the CompanionMatrixRootFinder to abstact class missing the findEigenvalues functions
- Added implementation for CompanionMatrixRootFinder using eigen library
- Added factory for creating object - CATCH or ANCAS 
- Added implementation of the CompanionMatrixRootsFinder using the armadillo library

#### Added 23/02/2024
- Added changelog
- Created the testedObcApp project
- Added main for windows
- Added the main process class
- Added timer interface and implementation for windows
- Added Versions file for the corrent version
- Added Test Manager class
- Added Defines for the required test parameters

