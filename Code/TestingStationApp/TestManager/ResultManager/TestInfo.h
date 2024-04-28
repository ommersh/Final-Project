
#ifndef TESTINFO_H
#define TESTINFO_H

#include "TestStatus.h"
#include "TestRecipe.h"
#include "SatelliteDataFormatEnum.h"


struct TestInfo {
    TestRecipe recipe;
    TestStatus status;

    //Real Results
    double realTCA;
    double realDistance;

    //Test Results
    //The results and run time of the first run
    double timeOfTcaFromStartingPointSec;
    double distanceOfTcaKM;
    int numberOfPointsTheAlgUsed;
    double runTimeMicro;

    //Repeated tests results - the iterations
    double avgRunTimeMicro;
    double minRunTimeMicro;


    SatelliteDataFormat format;
    char firstElemData[180]; //send elstrac
    char secondElemData[180];

};

#endif 