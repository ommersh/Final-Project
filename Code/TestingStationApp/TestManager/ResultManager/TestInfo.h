
#ifndef TESTINFO_H
#define TESTINFO_H

#include "TestStatus.h"
#include "TestRecipe.h"
#include "SatelliteDataFormatEnum.h"


struct TestInfo {
    char name[MAX_TEST_NAME_SIZE];
    TestStatus status;
    CommonTestRecipe recipe;

    //STRUCT TCA exist
    double realTCA;
    double realDistance;
    //struct testResult
    double distance;
    double TCA;
    double runTime;
    SatelliteDataFormat format;
    char firstElemData[80]; //send elstrac
    char secondElemData[80];

};

#endif 