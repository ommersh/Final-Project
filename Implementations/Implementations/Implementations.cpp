// Implementations.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "CATCH.h"
#include "FileReader.h"
#include "ANCAS.h"
#include <vector>
#include <chrono>


double getCurrentTymeInMicroSec();
void runCatch();
void runAncas();
int main()
{
    runCatch();
    runAncas();
    return 0;
}

void runCatch()
{
    //run catch
    CATCH c;
    FileReader fr;
    sFileData fileData = fr.readDataFromFile("../../SGP4/data/LEMUR2_COSMOS_GAUSS.csv");
    //printData(fileData);
    double* timePoints = fileData.timePoints;
    int lastPointIndex = fileData.size;
    double startTime, endTime;
    startTime = getCurrentTymeInMicroSec();
    TCA tca = c.CatchAlgorithm(fileData.data, timePoints, lastPointIndex);
    endTime = getCurrentTymeInMicroSec();
    std::cout << "Catch result:\nTime: " << tca.time << "\nDistance:" << tca.distance << "\n";
    std::cout << "Catch took:\n " << endTime - startTime << " micro seconds\n" << (endTime - startTime) / 1000000 << " seconds \n";
    if (fileData.data != nullptr)
    {
        delete[] fileData.data;
    }
}
void runAncas()
{
    //run ancas
    ANCAS a;
    FileReader fr;
    sFileData fileData = fr.readDataFromFile("../../SGP4/data/LEMUR2_COSMOS_GAUSS.csv");
    double* timePoints = fileData.timePoints;
    int lastPointIndex = fileData.size;
    double startTime, endTime;
    double temp;
    startTime = getCurrentTymeInMicroSec();
    TCA tca = a.ANCASAlgorithm(fileData.data , timePoints, lastPointIndex);
    endTime = getCurrentTymeInMicroSec();
    std::cout << "Ancas result:\nTime: " << tca.time << "\nDistance:" << tca.distance << "\n";
    std::cout << "Ancas took:\n " << endTime - startTime << " micro seconds\n" << (endTime - startTime) / 1000000 << " seconds \n";
    if (fileData.data != nullptr)
    {
        delete[] fileData.data;
    }
}

double getCurrentTymeInMicroSec()
{
    // Get the current time point
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    // Get the duration in microseconds
    auto duration = now.time_since_epoch();
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    return (double)microseconds;
}
