// Implementations.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "CompanionMatrixRootsFinderEigen.h"
#include "CompanionMatrixRootsFinderArmadillo.h"

#include "FileReader.h"
#include "CATCH.h"
#include "ANCAS.h"

#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>

double getCurrentTimeInMicroSec();
void runCatch(int degree);
void runCatchWithArmadillo(int degree);

void runAncas();
void startPrint();
void printResult(string algName, int degree, string testName, int numberOfPoints, double runTime, TCA tca);

int main()
{
    startPrint();
    runAncas();
    runCatch(15);
    runCatchWithArmadillo(15);

    return 0;
}
void runCatchWithArmadillo(int degree)
{
    //run catch
    CompanionMatrixRootsFinderArmadillo rootsFinder;
    CATCH c;
    c.init(&rootsFinder, degree);
    FileReader fr;
    sFileData fileData = fr.readDataFromFile("../LEMUR2_COSMOS_GAUSS.csv");

    //std::cout << "CATCH Data Size:" << ((sizeof(sPointData)* fileData.size)/1024.0) <<"KB"<< std::endl;

    //printData(fileData);
    double* timePoints = fileData.timePoints;
    int lastPointIndex = fileData.size;
    double startTime, endTime;
    startTime = getCurrentTimeInMicroSec();
    TCA tca = c.runAlgorithm(fileData.data, timePoints, lastPointIndex);
    endTime = getCurrentTimeInMicroSec();
    //std::cout << "Catch result:\nTime: " << tca.time << "\nDistance:" << tca.distance << "\n";
    //std::cout << "Catch took:\n " << endTime - startTime << " micro seconds\n" << (endTime - startTime) / 1000000 << " seconds \n";
    printResult("CATCH_Armadillo", degree, "LEMUR2_COSMOS", tca.numberOfPoints, endTime - startTime, tca);
    if (fileData.data != nullptr)
    {
        delete[] fileData.data, fileData.timePoints;
    }
}
void runCatch(int degree)
{
    //run catch
    CompanionMatrixRootsFinderEigen rootsFinder;
    CATCH c;
    c.init(&rootsFinder, degree);
    FileReader fr;
    sFileData fileData = fr.readDataFromFile("../LEMUR2_COSMOS_GAUSS.csv");

    //std::cout << "CATCH Data Size:" << ((sizeof(sPointData)* fileData.size)/1024.0) <<"KB"<< std::endl;

    //printData(fileData);
    double* timePoints = fileData.timePoints;
    int lastPointIndex = fileData.size;
    double startTime, endTime;
    startTime = getCurrentTimeInMicroSec();
    TCA tca = c.runAlgorithm(fileData.data, timePoints, lastPointIndex);
    endTime = getCurrentTimeInMicroSec();
    //std::cout << "Catch result:\nTime: " << tca.time << "\nDistance:" << tca.distance << "\n";
    //std::cout << "Catch took:\n " << endTime - startTime << " micro seconds\n" << (endTime - startTime) / 1000000 << " seconds \n";
    printResult("CATCH", degree, "LEMUR2_COSMOS", tca.numberOfPoints, endTime - startTime, tca);
    if (fileData.data != nullptr)
    {
        delete[] fileData.data, fileData.timePoints;
    }
}
void runAncas()
{
    //run ancas
    ANCAS a;
    FileReader fr;
    sFileData fileData = fr.readDataFromFile("../LEMUR2_COSMOS_CONST.csv");

    //std::cout << "ANCAS Data Size:" << ((sizeof(sPointData) * fileData.size) / 1024.0) << "KB" << std::endl;

    double* timePoints = fileData.timePoints;
    int lastPointIndex = fileData.size;
    double startTime, endTime;
    double temp;
    startTime = getCurrentTimeInMicroSec();
    TCA tca = a.runAlgorithm(fileData.data, timePoints, lastPointIndex);
    endTime = getCurrentTimeInMicroSec();
    //std::cout << "Ancas result:\nTime: " << tca.time << "\nDistance:" << tca.distance << "\n";
    //std::cout << "Ancas took:\n " << endTime - startTime << " micro seconds\n" << (endTime - startTime) / 1000000 << " seconds \n";
    printResult("ANCAS",3, "LEMUR2_COSMOS", tca.numberOfPoints, endTime - startTime, tca);
    if (fileData.data != nullptr)
    {
        delete[] fileData.data, fileData.timePoints;
    }
}

void printResult(string algName,int degree, string testName, int numberOfPoints, double runTime, TCA tca)
{
    std::cout << std::left << std::setw(15) << testName
        << "|" << std::left << std::setw(15) << algName
        << "|" << std::left << std::setw(15) << degree
        << "|" << std::left << std::setw(20) << numberOfPoints
        << "|" << std::left << std::setw(20) << runTime / 1000000
        << "|" << std::left << std::setw(20) << runTime
        << "|" << std::left << std::setw(15) << tca.distance
        << "|" << std::left << std::setw(15) << tca.time << std::endl;

}
void startPrint()
{
    std::cout << std::left << std::setw(15) << "testName"
        << "|" << std::left << std::setw(15) << "algName"
        << "|" << std::left << std::setw(15) << "degree"
        << "|" << std::left << std::setw(20) << "numberOfPoints"
        << "|" << std::left << std::setw(20) << "runTime(sec)"
        << "|" << std::left << std::setw(20) << "runTime(microSec)"
        << "|" << std::left << std::setw(15) << "TCA distance"
        << "|" << std::left << std::setw(15) << "TCA time" << std::endl;


    std::cout << std::left << std::setw(15) << std::setfill('-') << ""
        << "|" << std::left << std::setw(15) << ""
        << "|" << std::left << std::setw(15) << ""
        << "|" << std::left << std::setw(20) << ""
        << "|" << std::left << std::setw(20) << ""
        << "|" << std::left << std::setw(20) << ""
        << "|" << std::left << std::setw(15) << ""
        << "|" << std::left << std::setw(15) << "" << std::setfill(' ') << std::endl;

}
double getCurrentTimeInMicroSec()
{
    // Get the current time point
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    // Get the duration in microseconds
    auto duration = now.time_since_epoch();
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(duration).count();
    return (double)microseconds;
}
