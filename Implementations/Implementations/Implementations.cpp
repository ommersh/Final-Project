// Implementations.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "CATCH.h"
#include "FileReader.h"
#include "ANCAS.h"
#include <vector>
#include <chrono>




int GetIndexInTime(double time)
{
    double times[16] = {
        0,
        30.69148012,
        121.4245555,
        268.2337554,
        464.7028134,
        702.2450887,
        970.4788441,
        1257.680977,
        1551.299377,
        1838.501511,
        2106.735266,
        2344.277541,
        2540.746599,
        2687.555799,
        2778.288875,
        2808.980355 };
    double diff = INFINITY;
    int index = 0;
    for (int j = 0; j < N; j++)
    {
        if (abs(times[j] - time) < diff)
        {
            index = j;
            diff = abs(times[j] - time);
        }
    }
    return index;
}
class TestFunctionR1 : public VectorFunction
{
public:
    TestFunctionR1(sFileData* data)
    {
        filedata = data;
    }
    Vector3d getValue(int i)
    {
        Vector3d v;
        v(0) = filedata->data[i].r1x;
        v(1) = filedata->data[i].r1y;
        v(2) = filedata->data[i].r1z;
        return v;
    }
    sFileData* filedata;
};
class TestFunctionR2 : public VectorFunction
{
public:
    TestFunctionR2(sFileData* data)
    {
        filedata = data;
    }
    Vector3d getValue(int i)
    {
        Vector3d v;
        v(0) = filedata->data[i].r2x;
        v(1) = filedata->data[i].r2y;
        v(2) = filedata->data[i].r2z;
        return v;
    }
    sFileData* filedata;
};
class TestFunctionV1 : public VectorFunction
{
public:
    TestFunctionV1(sFileData* data)
    {
        filedata = data;
    }
    Vector3d getValue(int i)
    {
        Vector3d v;
        v(0) = filedata->data[i].v1x;
        v(1) = filedata->data[i].v1y;
        v(2) = filedata->data[i].v1z;
        return v;
    }
    sFileData* filedata;
};
class TestFunctionV2 : public VectorFunction
{
public:
    TestFunctionV2(sFileData* data)
    {
        filedata = data;
    }
    Vector3d getValue(int i)
    {
        Vector3d v;
        v(0) = filedata->data[i].v2x;
        v(1) = filedata->data[i].v2y;
        v(2) = filedata->data[i].v2z;
        return v;
    }
    sFileData* filedata;
};

double getCurrentTymeInMicroSec();
void runCatch();
void runAncas();
void printData(sFileData fileData)
{
    for (int i = 0; i < fileData.size; i++)
    {
        std::cout << i << ") " << fileData.timePoints[i] << "\n";
    }
}

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
    sFileData fileData = fr.readDataFromFile("../LEMUR_COSMOS_GAUSS.csv");
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
    sFileData fileData = fr.readDataFromFile("../LEMUR_COSMOS_CONST.csv");
    TestFunctionR1 r1(&fileData);
    TestFunctionR2 r2(&fileData);
    TestFunctionV1 v1(&fileData);
    TestFunctionV2 v2(&fileData);
    double* timePoints = fileData.timePoints;
    int lastPointIndex = fileData.size;
    double startTime, endTime;
    double temp;
    startTime = getCurrentTymeInMicroSec();
    TCA tca = a.ANCASAlgorithm(&r1, &r2, &v1, &v2, timePoints, lastPointIndex);
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
